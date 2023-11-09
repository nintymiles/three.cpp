#include "mtl_loader.h"

#include "texture.h"
#include "mesh_phong_material.h"
#include "texture_loader.h"

#include <stdlib.h>
//使用stbi_load时必须加上此宏定义，否则会出现 -- error LNK2019: 无法解析的外部符号 _stbi_load的错误
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>


using namespace std;

MTLLoader::MaterialCreator::MaterialCreator(string path, MaterialCreatorOptions options)
    :options(options) {
    filePath = path;//std::filesystem::path(path).parent_path();
    this->side = options.isEmpty() ? Side::FrontSide : options.side;
    this->wrap = options.isEmpty() ? Wrapping::RepeatWrapping : options.wrap;
}

MTLLoader::MaterialCreator& MTLLoader::MaterialCreator::setCrossOrigin(string value){
    this->crossOrigin = value;
    return *this;
}

void MTLLoader::MaterialCreator::setMaterials(unordered_map<string, MaterialsInfoMap>& materialsInfo){

    this->materialsInfo = materialsInfo;
}

unordered_map<string, MTLLoader::MaterialsInfoMap> MTLLoader::MaterialCreator::convert(unordered_map<string, MaterialsInfoMap>& materialsInfo){
    if (this->options.isEmpty()) return materialsInfo;

    unordered_map<string, MaterialsInfoMap> converted;

    for (auto key = materialsInfo.begin();key != materialsInfo.end();) {
        MaterialsInfoMap mat = key->second;

        converted.insert({ key->first, MaterialsInfoMap() });

        MaterialsInfoMap& covmat = converted[key->first];

        MaterialInfo convertedMaterialInfo;

        for (auto key1 = mat.begin();key1 != mat.end();) {
            bool save = true;
            auto& value = mat[key1->first].value;
            MTLName lprop = key1->first;

            if (lprop == MTLName::kd || lprop == MTLName::ka || lprop == MTLName::ks) {
                if (!this->options.isEmpty() && this->options.normalizedRGB) {
                    value[0] = value[0] / 255.0f;
                    value[1] = value[1] / 255.0f;
                    value[2] = value[2] / 255.0f;
                }
                if (!this->options.isEmpty() && this->options.ignoreZeroRGBs) {
                    if (value[0] == 0 && value[1] == 0 && value[2] == 0) {
                        save = false;
                    }
                }
            }
            if (save) {
                std::memcpy(convertedMaterialInfo.value, value, 3);

                covmat.insert({ lprop,convertedMaterialInfo });
            }
            key1++;
        }
        key++;
    }
    return converted;
}

void MTLLoader::MaterialCreator::setMapForType(MTLParameters& parameter, MTLMapType mapType, const string& value){
    //if ( params[ mapType ] ) return; // Keep the first encountered texture

    auto texParams = getTextureParams(value, parameter);
    auto texFilePath = filePath + threecpp::getFileSeparator() + texParams.url;
    if(threecpp::FileExists(texFilePath)) {
        auto map = loadTexture(texFilePath, TextureMapping::Unknown);

        map->repeat.copy(texParams.scale);
        map->offset.copy(texParams.offset);

//        map->wrapS = this->wrap;
//        map->wrapT = this->wrap;

        switch (mapType) {
            case MTLMapType::map:
                parameter.map = map;
                break;
            case MTLMapType::specularMap:
                parameter.specularMap = map;
                break;
            case MTLMapType::emissiveMap:
                parameter.emissiveMap = map;
                break;
            case MTLMapType::normalMap:
                parameter.normalMap = map;
                break;
            case MTLMapType::bumpMap:
                parameter.bumpMap = map;
                break;
            case MTLMapType::alphaMap:
                parameter.alphaMap = map;
                break;
        }
    }
}

MTLLoader::TexParams MTLLoader::MaterialCreator::getTextureParams(const string& value, MTLParameters& matParams){
    TexParams texParams;
    texParams.scale.set(1, 1);
    texParams.offset.set(0, 0);

    std::string match;
    std::string localV = value;
    int pos = -1;
    vector<string> items = string_utils::split(localV,' ');

    for (int i = 0;i < items.size();i++) {
        if (items[i].find("-bm", 0) != string::npos) {
            pos = i;
            break;
        }
    }

    if (pos >= 0) {
        char* pEnd;
        matParams.bumpScale = strtof(items[pos + 1].c_str(), &pEnd);
        vector<string>::iterator iter = items.begin();
        int start = pos;
        int end = pos + 2;
        items.erase(iter + start - 1, iter + end - 1);
    }

    for (int i = 0;i < items.size();i++) {
        if (items[i].find("-s", 0) != string::npos) {
            pos = i;
            break;
        }
        pos = -1;
    }

    if (pos >= 0) {
        char* pEnd;
        float x = strtof(items[pos + 1].c_str(), &pEnd);
        float y = strtof(items[pos + 2].c_str(), &pEnd);
        texParams.scale.set(x, y);
        vector<string>::iterator iter = items.begin();
        int start = pos;
        int end = pos + 4;
        items.erase(iter + start - 1, iter + end - 1);
    }

    for (int i = 0;i < items.size();i++) {
        if (items[i].find("-o", 0) != string::npos) {
            pos = i;
            break;
        }
        pos = -1;
    }

    if (pos >= 0) {
        char* pEnd;
        float x = strtof(items[pos + 1].c_str(), &pEnd);
        float y = strtof(items[pos + 2].c_str(), &pEnd);
        texParams.offset.set(x, y);
        vector<string>::iterator iter = items.begin();
        int start = pos;
        int end = pos + 4;
        items.erase(iter + start - 1, iter + end - 1);
    }

    std::string s;
    for (const auto& piece : items) s += piece;

    texParams.url = s;

    return texParams;
}

Material::sptr MTLLoader::MaterialCreator::create(const string& materialName){
    if (this->materials.count(materialName) == 0) {
        createMaterial(materialName);
    }

    return materials[materialName];
}

Material::sptr MTLLoader::MaterialCreator::createMaterial(const string& materialName){
    auto mat = materialsInfo[materialName];

    MTLParameters parameter;
    parameter.name = materialName;
    parameter.side = this->side;

    float n;
    for (auto key = mat.begin();key != mat.end();) {
        auto materialInfo = key->second;
        //if (materialInfo.isEmpty()) continue;

        switch (key->first) {
        case MTLName::kd:
            // Diffuse color (color under white light) using RGB values
            parameter.color.fromArray(materialInfo.value, 3).convertSRGBToLinear();
            break;
        case MTLName::ks:
            // Specular color (color when light is reflected from shiny surface) using RGB values
            parameter.specular.fromArray(materialInfo.value, 3).convertSRGBToLinear();
            break;
        case MTLName::ke:
            // Emissive using RGB values
            parameter.emissive.fromArray(materialInfo.value, 3).convertSRGBToLinear();
            break;
        case MTLName::map_kd:
            // Diffuse texture map
            setMapForType(parameter, MTLMapType::map, materialInfo.stringValue);
            break;
        case MTLName::map_ks:
            setMapForType(parameter, MTLMapType::specularMap, materialInfo.stringValue);
            break;
        case MTLName::map_ke:
            setMapForType(parameter, MTLMapType::emissiveMap, materialInfo.stringValue);
            break;
        case MTLName::norm:
            setMapForType(parameter, MTLMapType::normalMap, materialInfo.stringValue);
            break;
        case MTLName::map_bump:
        case MTLName::bump:
            setMapForType(parameter, MTLMapType::bumpMap, materialInfo.stringValue);
            break;
        case MTLName::map_d:
            setMapForType(parameter, MTLMapType::alphaMap, materialInfo.stringValue);
            break;
        case MTLName::ns:
            parameter.shininess = atof(materialInfo.stringValue.c_str());
            break;
        case MTLName::ni:
            parameter.refractionRatio = atof(materialInfo.stringValue.c_str());
            break;
        case MTLName::d:
            n = atof(materialInfo.stringValue.c_str());

            if (n < 1) {
                parameter.opacity = n;
                parameter.transparent = true;
            }
            break;
        case MTLName::tr:
            n = atof(materialInfo.stringValue.c_str());
            if (this->options.invertTrpproperty) n = 1 - n;
            if (n > 0) {
                parameter.opacity = 1 - n;
                parameter.transparent = true;
            }
            break;
        default:
            break;
        }
        key++;
    }

    MeshPhongMaterial::sptr material = MeshPhongMaterial::create();
    material->name = parameter.name;
    material->side = parameter.side;
    material->color = parameter.color;
    material->specular = parameter.specular;
    material->emissive = parameter.emissive;
    material->map = parameter.map;
    material->specularMap = parameter.specularMap;
    material->emissiveMap = parameter.emissiveMap;
    material->normalMap = parameter.normalMap;
    material->bumpMap = parameter.bumpMap;
    material->alphaMap = parameter.alphaMap;
    material->bumpScale = parameter.bumpScale;
    material->shininess = parameter.shininess;
    material->opacity = parameter.opacity;
    material->refractionRatio = parameter.refractionRatio;
    material->transparent = parameter.transparent;

    this->materials[materialName] = material;

    return material;
}

Texture::sptr MTLLoader::MaterialCreator::loadTexture(string filePath, TextureMapping mapping){
    Texture::sptr texture = TextureLoader::load((char*)filePath.c_str());
    if (mapping != TextureMapping::Unknown) texture->mapping = mapping;

    return texture;
}

MTLLoader::MaterialCreator::sptr MTLLoader::parse(const string& text) {
    auto lines = string_utils::split(text,'\n' );

    unordered_map<string, MaterialsInfoMap> materialsInfo{};
    //unordered_map<MTLName, string> info{};
    //MaterialInfo matInfo{};
    const char delimiter_pattern = ' ';

    std::string materialName;
    for(int li = 0;li < lines.size();li++){
        auto line = lines[li];
        string_utils::trim(line);

        if(line.empty() || line[0] == '#'){
            // Blank line or comment ignore
            continue;
        }
        auto pos = line.find(' ');
        auto key = (pos>=0)?line.substr(0,pos):line;
        string_utils::toLowerCase(key);

        auto value = (pos>=0)?line.substr(pos+1):"";
        string_utils::ltrim(value);

        if(key == "newmtl"){
            // New material , value = material name
            materialName = value;
            materialsInfo[materialName] = MaterialsInfoMap{};
        }else{
            MTLName mtlName;
            MaterialInfo mInfo{};
            MaterialsInfoMap &currentInfoMap = materialsInfo[materialName];
            if(key=="ka" || key=="kd" || key == "ks" || key == "ke"){
                auto ss = string_utils::split(value,delimiter_pattern);


                mInfo.value[0]= string_utils::parseFloat(ss[0]);
                mInfo.value[1]= string_utils::parseFloat(ss[1]);
                mInfo.value[2]= string_utils::parseFloat(ss[2]);
//                MaterialValue mVal{};
//                mVal.value[0]= string_utils::parseFloat(ss[0]);
//                mVal.value[1]= string_utils::parseFloat(ss[1]);
//                mVal.value[2]= string_utils::parseFloat(ss[2]);
//                std::vector<float> propVal{string_utils::parseFloat(ss[0]),string_utils::parseFloat(ss[1]),string_utils::parseFloat(ss[2])};


                if(key=="ka") {
                    mtlName = MTLName::ka;
                }
                if(key=="kd") {
                    mtlName = MTLName::kd;
                }
                if(key=="ks") {
                    mtlName = MTLName::ks;
                }
                if(key=="ke") {
                    mtlName = MTLName::ke;
                }
                currentInfoMap[mtlName] = mInfo;

            }else{
                if(key=="d") {
                    mtlName = MTLName::d;
                    mInfo.stringValue = value;
                    currentInfoMap[mtlName] = mInfo;
                }
                if(key=="illum") {
                    mtlName = MTLName::illum;
                    mInfo.stringValue = value;
                    currentInfoMap[mtlName] = mInfo;
                }
                if(key=="ns") {
                    mtlName = MTLName::ns;
                    mInfo.stringValue = value;
                    currentInfoMap[mtlName] = mInfo;
                }
                if(key=="ni") {
                    mtlName = MTLName::ni;
                    mInfo.stringValue = value;
                    currentInfoMap[mtlName] = mInfo;
                }
                if(key=="map_kd"){
                    mtlName = MTLName::map_kd;
                    mInfo.stringValue = value;
                    currentInfoMap[mtlName] = mInfo;
                }

            }


        }

    }


    MaterialCreator::sptr materialCreator = MaterialCreator::create();//std::make_shared<MaterialCreator>(text,materialOptions);
    materialCreator->setCrossOrigin( crossOrigin );
    materialCreator->setMaterials( materialsInfo );
    materialCreator->filePath = mtlFilePath;
    multiMaterialCreator = materialCreator;
    return materialCreator;
    //return nullptr;
}

MTLLoader::MaterialCreator::sptr MTLLoader::load(const string& path) {
    mtlFilePath = threecpp::getFileDir(path);

    filesystem::path absoluteFilePath = filesystem::absolute(path);

    string filePath = absoluteFilePath.parent_path().string();

    ifstream infile;

    infile.open(path);

    stringstream ss;
    ss << infile.rdbuf();
    string textAll = ss.str();

    infile.close();

    return parse(textAll);
}

