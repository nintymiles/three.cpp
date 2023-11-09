#include "obj_loader.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdio>

//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "buffer_geometry.h"
#include "material.h"
#include "mesh.h"
#include "line.h"
#include "points.h"
#include "mesh_phong_material.h"
#include "line_basic_material.h"
#include "points_material.h"
#include "string_utils.h"
#include "timer.h"
#include "common_utils.h"

#include <unordered_set>

namespace obj_loader {
    Object::ObjectMaterial::sptr emptyMaterial = nullptr;
    static const int OBJ_SIZE = 1000 * 1000 * 1000 * 6;

    // o object_name | g group_name
    const std::string _object_pattern = R"(^[og]\s*(.+)?)";
    // mtllib file_reference
    const std::string _material_library_pattern = R"(^mtllib )";
    // usemtl material_name
    const std::string _material_use_pattern = R"(^usemtl )";
    // usemap map_name
    const std::string _map_use_pattern = R"(^usemap )";
    const std::string _face_vertex_data_separator_pattern = R"(\s+)";

    Vector3 _vA;
    Vector3 _vB;
    Vector3 _vC;

    Vector3 _ab;
    Vector3 _cb;

    Color _color;
}
using namespace obj_loader;
using namespace std;
using namespace string_utils;

Object::ObjectMaterial::sptr Object::ObjectMaterial::clone(int _index){
    auto cloned = std::make_shared<Object::ObjectMaterial>();
    cloned->name = name;
    cloned->index = _index;
    cloned->smooth = smooth;

    cloned->objMtlLib = objMtlLib;

    return cloned;
}

Object::ObjectMaterial::sptr& Object::currentMaterial(){
    if (materials.size() > 0)
        return materials[materials.size() - 1];
    return emptyMaterial;

}

Object::ObjectMaterial::sptr Object::startMaterial(const string& name, vector<string>& libraries){
    auto previous = finalize(false);

    if (previous!=nullptr && (previous->inherited || previous->groupCount <= 0)){
        materials.erase(materials.begin() + previous->index);
    }

    Object::ObjectMaterial::sptr material = make_shared<Object::ObjectMaterial>();

    material->index = materials.size();
    material->name = name;
    material->stringLib = libraries[libraries.size() - 1];
    material->smooth = previous!=nullptr ? previous->smooth : smooth;
    material->groupStart = previous!=nullptr ? previous->groupEnd : 0;
    material->groupEnd = -1;
    material->groupCount = -1;
    material->inherited = false;

    materials.push_back(material);

    return material;
}


Object::ObjectMaterial::sptr& Object::finalize(bool end){
    ObjectMaterial::sptr& lastMultiMaterial = this->currentMaterial();
    if (lastMultiMaterial!=nullptr && lastMultiMaterial->groupEnd == -1) {
        lastMultiMaterial->groupEnd = geometry.vertices.size() / 3;
        lastMultiMaterial->groupCount = lastMultiMaterial->groupEnd - lastMultiMaterial->groupStart;
        lastMultiMaterial->inherited = false;
    }

    // Ignore objects tail materials if no face declarations followed them before a new o/g started.
    if (end && materials.size() > 1) {

        for (int mi = (int)materials.size() - 1; mi >= 0; mi--) {

            if (materials[mi]->groupCount <= 0) {
                materials.erase(materials.begin() + mi);
            }
        }
    }

    // Guarantee at least one empty material, this makes the creation later more straight forward.
    if (end && materials.size() == 0) {
        Object::ObjectMaterial::sptr material = make_shared<Object::ObjectMaterial>();
        material->name = "";
        material->smooth = smooth;
        materials.push_back(material);

    }

    return lastMultiMaterial;
}

int ObjectState::parseVertexIndex(int value, size_t len){
    int index = value;

    return (index >= 0 ? index - 1 : index + len / 3) * 3;
}

int ObjectState::parseNormalIndex(int value, size_t len){
    int index = value;
    return (index >= 0 ? index - 1 : index + len / 3) * 3;
}

int ObjectState::parseUVIndex(int value, size_t len){
    int index = value;

    return (index >= 0 ? index - 1 : index + len / 2) * 2;
}

void addVector3(vector<float>& target, float x, float y, float z) {
    target.push_back(x);
    target.push_back(y);
    target.push_back(z);
}

void addVector2(vector<float>& target, float x, float y) {
    target.push_back(x);
    target.push_back(y);
}

void ObjectState::addVertex(int a, int b, int c){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;


    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
    addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
}

void ObjectState::addColor(int a, int b, int c){
    auto& src = colors;
    auto& dst = object->geometry.colors;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

//    if ( src[ a ] !== undefined ) dst.push( src[ a + 0 ], src[ a + 1 ], src[ a + 2 ] );
//    if ( src[ b ] !== undefined ) dst.push( src[ b + 0 ], src[ b + 1 ], src[ b + 2 ] );
//    if ( src[ c ] !== undefined ) dst.push( src[ c + 0 ], src[ c + 1 ], src[ c + 2 ] );

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
    addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
    addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
}

void ObjectState::addVertexColor(int a){
    auto& src = colors;
    auto& dst = object->geometry.colors;

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
}

void ObjectState::addUV(int a, int b, int c){
    auto& src = uvs;
    auto& dst = object->geometry.uvs;


   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 2, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 2, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 2, dst.end()); */

    //todo:fix this
    if(src.size()>0) {
        addVector2(dst, src[a + 0], src[a + 1]);
        addVector2(dst, src[b + 0], src[b + 1]);
        addVector2(dst, src[c + 0], src[c + 1]);
    }else{
//        addVector2(dst, 0, 0);
//        addVector2(dst, 0, 0);
//        addVector2(dst, 0, 0);
    }

}

void ObjectState::addDefaultUV(){
    auto& dst = object->geometry.uvs;

    addVector2(dst,0,0);
    addVector2(dst,0,0);
    addVector2(dst,0,0);
}



void ObjectState::addNormal(int a, int b, int c){
    auto& src = normals;
    auto& dst = object->geometry.normals;

   /* if (src.size() != dst.size())
        dst.resize(src.size());

    std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
    std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
    std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/

    //todo:fix this
    if(normals.size() > 0) {
        addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
        addVector3(dst, src[b + 0], src[b + 1], src[b + 2]);
        addVector3(dst, src[c + 0], src[c + 1], src[c + 2]);
    }else{
//        addVector3(dst, 0, 0, 0);
//        addVector3(dst, 0, 0, 0);
//        addVector3(dst, 0, 0, 0);
    }


}

void ObjectState::addFaceNormal(int a, int b, int c){
    auto& src = normals;
    auto& dst = object->geometry.normals;

    _vA.fromArray( src, a );
    _vB.fromArray( src, b );
    _vC.fromArray( src, c );

    _cb.subVectors( _vC, _vB );
    _ab.subVectors( _vA, _vB );
    _cb.cross( _ab );

    _cb.normalize();

    addVector3( dst,_cb.x, _cb.y, _cb.z );
    addVector3( dst,_cb.x, _cb.y, _cb.z );
    addVector3( dst,_cb.x, _cb.y, _cb.z );

}

ObjectState::ObjectState(){
    object = make_shared<Object>();
}

void ObjectState::startObject(const string& name, bool fromDeclaration){
    // If the current object (initial from reset) is not from a g/o declaration in the parsed
    // file. We need to use it for the first parsed g/o to keep things in sync.
    if (this->object!=nullptr && this->object->fromDeclaration == false){
        this->object->name = name;
        this->object->fromDeclaration = fromDeclaration != false;

        return;
    }

    auto previousMaterial = object->currentMaterial();

    if (this->object!=nullptr){
        this->object->finalize(true);
    }

    this->object = make_shared<Object>();
    this->object->name = name;
    this->object->fromDeclaration = fromDeclaration != false;

    // Inherit previous objects material.
    // Spec tells us that a declared material must be set to all objects until a new material is declared.
    // If a usemtl declaration is encountered while this new object is being parsed, it will
    // overwrite the inherited material. Exception being that there was already face declarations
    // to the inherited material, then it will be preserved for proper MultiMaterial continuation.
    if (previousMaterial!=nullptr && previousMaterial->name != ""){
        Object::ObjectMaterial::sptr declared = previousMaterial->clone(0);
        declared->inherited = true;
        object->materials.push_back(declared);
    }

    this->objects.push_back(this->object);
}

void ObjectState::finalize(){
    if (object!=nullptr)
    {
        object->finalize(true);
    }
}

////adapting tinyobj
//void ObjectState::addVertexPointFromArray(float src[3]){
//    auto& dst = object->geometry.vertices;
//
//    /* if (src.size() != dst.size())
//         dst.resize(src.size());
//
//     std::copy(src.begin() + a, src.begin() + a + 2, dst.end());*/
//
//    addVector3(dst, src[0], src[1], src[2]);
//
//}
//
//void ObjectState::addNormalFromArray(float src[3]){
//    auto& dst = object->geometry.normals;
//
//    /* if (src.size() != dst.size())
//         dst.resize(src.size());
//
//     std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
//     std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
//     std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/
//
//    addVector3(dst, src[0], src[1], src[2]);
//
//}
//
//void ObjectState::addUVFromArray(float src[2]){
//    auto& dst = object->geometry.uvs;
//
//    /* if (src.size() != dst.size())
//         dst.resize(src.size());*/
//
//    addVector2(dst, src[0], src[1]);
//}
//
//void ObjectState::addColorFromFloat(float a, float b, float c) {
//    auto& dst = object->geometry.colors;
//
//    /* if (src.size() != dst.size())
//         dst.resize(src.size());
//
//     std::copy(src.begin() + a, src.begin() + a + 3, dst.end());
//     std::copy(src.begin() + b, src.begin() + b + 3, dst.end());
//     std::copy(src.begin() + c, src.begin() + c + 3, dst.end());*/
//
//    addVector3(dst, a , b, c);
//}

void ObjectState::addVertexPoint(int a){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
}

void ObjectState::addVertexLine(int a){
    auto& src = vertices;
    auto& dst = object->geometry.vertices;

    addVector3(dst, src[a + 0], src[a + 1], src[a + 2]);
}

void ObjectState::addUVLine(int a){
    auto& src = uvs;
    auto& dst = object->geometry.uvs;

    addVector2(dst, src[a + 0], src[a + 1]);
}

void ObjectState::addPointGeometry(const vector<string>& _vertices){
    object->geometry.type = "Points";

    auto vLen = vertices.size();

    for (auto vi = 0; vi < _vertices.size(); vi++){
        auto index  = parseVertexIndex(atoi(_vertices[vi].c_str()),vLen);
        addVertexPoint(index);
        addVertexColor(index);
    }
}

void ObjectState::addLineGeometry(const vector<string>& _vertices, const vector<string>& _uvs){
    object->geometry.type = "Line";

    auto vLen = vertices.size();
    auto uvLen = uvs.size();

    for (auto vi = 0; vi < _vertices.size(); vi++){
        addVertexLine(parseVertexIndex(atoi(_vertices[vi].c_str()), vLen));
    }

    if (_uvs.size()> 0){
        for (auto uvi = 0; uvi < _uvs.size(); uvi++){
            addUVLine(parseUVIndex(atoi(_uvs[uvi].c_str()), uvLen));
        }
    }
}

void OBJLoader::parseState(ObjectState& state){
    state.startObject("", false);
}

float OBJLoader::parseFloat(string value){
    return atof(value.c_str());
}

int OBJLoader::parseInt(string value){
    return atoi(value.c_str());
}

void ObjectState::addFace(int a, int b, int c, int ua, int ub, int uc, int na, int nb, int nc){
    auto vLen = vertices.size();

    auto ia = parseVertexIndex(a, vLen);
    auto ib = parseVertexIndex(b, vLen);
    auto ic = parseVertexIndex(c, vLen);

    addVertex(ia, ib, ic);

    if (colors.size() > 0){
        addColor(ia, ib, ic);
    }

    //normals
    if (na != numeric_limits<int>::quiet_NaN()){
        // Normals are many times the same. If so, skip function call and parseInt.
        auto nLen = normals.size();
        ia = parseNormalIndex(na, nLen);

        ib = na == nb ? ia : parseNormalIndex(nb, nLen);
        ic = na == nc ? ia : parseNormalIndex(nc, nLen);

        addNormal(ia, ib, ic);

    }else{
        addFaceNormal(ia,ib,ic);

    }

    //uvs
    if (ua != numeric_limits<int>::quiet_NaN()){
        auto uvLen = uvs.size();
        ia = parseUVIndex(ua, uvLen);
        ib = parseUVIndex(ub, uvLen);
        ic = parseUVIndex(uc, uvLen);

        addUV(ia, ib, ic);

        object->geometry.hasUVIndices = true;
    }else{
        // add placeholder values (for inconsistent face definitions)
        addDefaultUV();
    }
}

void ObjectState::addFace(int a, int b, int c, int d, int ua, int ub, int uc, int ud, int na, int nb, int nc, int nd){
    auto vLen = vertices.size();

    auto ia = parseVertexIndex(a, vLen);
    auto ib = parseVertexIndex(b, vLen);
    auto ic = parseVertexIndex(c, vLen);

    if (d == numeric_limits<int>::quiet_NaN()){
        addVertex(ia, ib, ic);
    }else{
        auto id = parseVertexIndex(d, vLen);

        addVertex(ia, ib, id);
        addVertex(ib, ic, id);
    }

    if (colors.size() > 0){

        addColor(ia, ib, ic);

    }

    if (ua != numeric_limits<int>::quiet_NaN()){

        auto uvLen = uvs.size();
        ia = parseUVIndex(ua, uvLen);
        ib = parseUVIndex(ub, uvLen);
        ic = parseUVIndex(uc, uvLen);

        if (d == numeric_limits<int>::quiet_NaN()){
            addUV(ia, ib, ic);
        }else{
            auto id = parseUVIndex(ud, uvLen);

            addUV(ia, ib, id);
            addUV(ib, ic, id);
        }

    }

    if (na != numeric_limits<int>::quiet_NaN()){

        // Normals are many times the same. If so, skip function call and parseInt.
        auto nLen = normals.size();
        ia = parseNormalIndex(na, nLen);

        ib = na == nb ? ia : parseNormalIndex(nb, nLen);
        ic = na == nc ? ia : parseNormalIndex(nc, nLen);

        if (d == numeric_limits<int>::quiet_NaN()){
            addNormal(ia, ib, ic);
        }else{
            auto id = parseNormalIndex(nd, nLen);

            addNormal(ia, ib, id);
            addNormal(ib, ic, id);
        }

    }
}

void OBJLoader::setMaterials(const MTLLoader::MaterialCreator& materials){
    this->materials = materials;
}

Group::sptr OBJLoader::parse(const string& path){
    Timer sw;

    ObjectState state;

    state.vertices.reserve(1000 * 64);
    state.normals.reserve(1000 * 64);
    state.uvs.reserve(1000 * 64);


    parseState(state);

    filesystem::path absoluteFilePath = filesystem::absolute(path);

    string filePath = absoluteFilePath.parent_path().string();

    ifstream infile;

    infile.open(path);

    stringstream ss;
    ss << infile.rdbuf();
    string textAll = ss.str();

    infile.close();

    if (textAll.find(R"(\r\n)") > -1) {
        textAll = replace_all(textAll, R"(\r\n)", R"(\n)");
    }
    if (textAll.find(R"(\\\n)") > -1) {
        textAll = replace_all(textAll, R"(\\\n)", "");
    }

    vector<string> lines = split(textAll, '\n');

    char lineFirstChar;

    auto intNan = numeric_limits<int>::quiet_NaN();

    vector<vector<string>> faceVertices;
    faceVertices.reserve(10000 * 64);
    vector<string> lineVertices;
    lineVertices.reserve(10000 * 64);
    vector<string> lineUVs;
    lineUVs.reserve(10000 * 64);

    for (int i = 0; i < lines.size(); i++) {
        //string line = trim(lines[i]);
        string& line = lines[i];
        //float x, y, z;

        if (line.size() == 0 || line.empty()) continue;

        lineFirstChar = line.c_str()[0];

        if (lineFirstChar == '#') continue;

        vector<string> lineData = split(line, ' ');
        if(lineData[0] == "v") {
            state.vertices.push_back(parseFloat(lineData[1]));
            state.vertices.push_back(parseFloat(lineData[2]));
            state.vertices.push_back(parseFloat(lineData[3]));
            if(lineData.size()>=7){
                state.colors.push_back(parseFloat(lineData[4]));
                state.colors.push_back(parseFloat(lineData[5]));
                state.colors.push_back(parseFloat(lineData[6]));
            }else{
                //state.colors.push_back(quite_nan)
            }
        }
        else if(lineData[0] == "vn") {
            state.normals.push_back(parseFloat(lineData[1]));
            state.normals.push_back(parseFloat(lineData[2]));
            state.normals.push_back(parseFloat(lineData[3]));
        }
        else if(lineData[0] == "vt") {
            state.uvs.push_back(parseFloat(lineData[1]));
            state.uvs.push_back(parseFloat(lineData[2]));
        }
        else if (lineData[0] == "f") {
            vector<string> vertexData = split(line, ' ');
            vertexData.erase(vertexData.begin());

            faceVertices.clear();

            // Parse the face vertex data into an easy to work with format
            if (vertexData.size() == 1)
                faceVertices[0] = vertexData;
            else {
                for (int j = 0; j < vertexData.size(); j++) {
                    auto& vertex = vertexData[j];
                    if (vertex.size() > 0) {
                        auto vertexParts = split(vertex, '/');
                        faceVertices.push_back(vertexParts);
                    }
                }
            }

            // Draw an edge between the first vertex and all subsequent vertices to form an n-gon
            auto& v1 = faceVertices[0];

            for (int j = 1; j < faceVertices.size() - 1; j++){
                auto& v2 = faceVertices[j];
                auto& v3 = faceVertices[j + 1];

                state.addFace(
                        parseInt(v1[0]), parseInt(v2[0]), parseInt(v3[0]),
                        v1.size() > 1 ? parseInt(v1[1]) : numeric_limits<int>::quiet_NaN(), v2.size() > 1 ? parseInt(v2[1]) : numeric_limits<int>::quiet_NaN(), v3.size() > 1 ? parseInt(v3[1]) : numeric_limits<int>::quiet_NaN(),
                        v1.size() > 2 ? parseInt(v1[2]) : numeric_limits<int>::quiet_NaN(), v2.size() > 2 ? parseInt(v2[2]) : numeric_limits<int>::quiet_NaN(), v3.size() > 2 ? parseInt(v3[2]) : numeric_limits<int>::quiet_NaN()
                );
            }
        }
        else if(lineData[0] == "l"){
            auto lineParts = split(line, ' ');
            lineParts.erase(lineParts.begin());
            lineVertices.clear();
            lineUVs.clear();

            if (line.find("/") == -1){
                lineVertices = lineParts;
            }else{
                for (int li = 0; li < lineParts.size(); li++){
                    auto parts = split(lineParts[li], '/');

                    if (parts[0] != "") lineVertices.push_back(parts[0]);
                    if (parts[1] != "") lineUVs.push_back(parts[1]);
                }
            }
            state.addLineGeometry(lineVertices, lineUVs);

        }
        else if(lineData[0] == "p"){
            auto lineD = line.substr(1);
            auto pointData = split(lineD, ' ');

            state.addPointGeometry(pointData);
        }
        else if(lineData[0] == "s"){
            auto result = split(line, ' ');
            if (result.size() > 1){
                toLowerCase(result[1]);
                state.object->smooth = (result[1] != "0" && result[1] != "off");
            }else{
                state.object->smooth = true;
            }
            auto& material = state.object->currentMaterial();
            if (material->index != -1) material->smooth = state.object->smooth;
            continue;
        }
        else if(lineData[0] == "s"){
            // smooth shading

            // @todo Handle files that have varying smooth values for a set of faces inside one geometry,
            // but does not define a usemtl for each face set.
            // This should be detected and a dummy material created (later MultiMaterial and geometry groups).
            // This requires some care to not create extra material on each smooth value for "normal" obj files.
            // where explicit usemtl defines geometry groups.
            // Example asset: examples/models/obj/cerberus/Cerberus.obj

            /*
             * http://paulbourke.net/dataformats/obj/
             *
             * From chapter "Grouping" Syntax explanation "s group_number":
             * "group_number is the smoothing group number. To turn off smoothing groups, use a value of 0 or off.
             * Polygonal elements use group numbers to put elements in different smoothing groups. For free-form
             * surfaces, smoothing groups are either turned on or off; there is no difference between values greater
             * than 0."
             */
            auto result = split(line, ' ');
            if (result.size() > 1){
                toLowerCase(result[1]);
                state.object->smooth = (result[1] != "0" && result[1] != "off");
            }else{
                // ZBrush can produce "s" lines #11707
                state.object->smooth = true;
            }
            auto& material = state.object->currentMaterial();
            if (material->index != -1) material->smooth = state.object->smooth;
            continue;
        }
        else if(lineData[0] == "o" || lineData[0] == "g"){
            // o object_name
            // or
            // g group_name

            // WORKAROUND: https://bugs.chromium.org/p/v8/issues/detail?id=2869
            // let name = result[ 0 ].slice( 1 ).trim();
            //todo:fix this?
//            const name = ( ' ' + result[ 0 ].slice( 1 ).trim() ).slice( 1 );
            auto name = line.substr(2);
            state.startObject(name, true);
            continue;
        }
        else if(lineData[0] == "usemtl") {
            // material
            string mtlName = line.substr(7);
            state.object->startMaterial(mtlName, state.materialLibraries);
            continue;
        }
        else if(lineData[0] == "mtllib") {
            // mtl file
            string matFileName = line.substr(7);
            string directory = filePath;
            string matPath = directory + threecpp::getFileSeparator() + matFileName;
            //state.object->startMaterial(matPath, state.materialLibraries);
            state.materialLibraries.push_back(matPath);
            continue;
        }
        else if(lineData[0] == "usemap"){
            // the line is parsed but ignored since the loader assumes textures are defined MTL files
            // (according to https://www.okino.com/conv/imp_wave.htm, 'usemap' is the old-style Wavefront texture reference method)

            std::cout << "THREE.OBJLoader: Rendering identifier \"usemap\" not supported. Textures must be defined in MTL files." << std::endl;
        }
        else{
            if (line == "\0")
                continue;
            else {
                throw std::runtime_error("THREECPP.OBJLoader: Unexpected line:" + line);
            }
        }




//        if (lineFirstChar == 'v') {
//            vector<string> data = split(line, ' ');
//
//            switch (line.c_str()[0]) {
//                case 'v':
//                    std::sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
//                    state.vertices.push_back(x);
//                    state.vertices.push_back(y);
//                    state.vertices.push_back(z);
//                    if(data.size()>=7){
//                        state.colors.push_back(parseFloat(data[4]));
//                        state.colors.push_back(parseFloat(data[5]));
//                        state.colors.push_back(parseFloat(data[6]));
//                    }else{
//                        //state.colors.push_back(quite_nan)
//                    }
//                    break;
//                default:
//                    break;
//            }
//
//            switch (line.c_str()[1]) {
//                case 'n':
//                    std::sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
//                    state.normals.push_back(x);
//                    state.normals.push_back(y);
//                    state.normals.push_back(z);
//                    break;
//                case 't':
//                    std::sscanf(line.c_str(), "vt %f %f", &x, &y);
//                    state.uvs.push_back(x);
//                    state.uvs.push_back(y);
//                    break;
//                default:
//                    break;
//            }
//        }
//    else if (lineFirstChar == 'f') {
//            vector<string> vertexData = split(line, ' ');
//            vertexData.erase(vertexData.begin());
//
//            faceVertices.clear();
//            if (vertexData.size() == 1) faceVertices[0] = vertexData;
//            else {
//                for (int j = 0; j < vertexData.size(); j++) {
//                    auto& vertex = vertexData[j];
//                    if (vertex.size() > 0) {
//                        auto vertexParts = split(vertex, '/');
//                        faceVertices.push_back(vertexParts);
//                    }
//                }
//            }
//            auto& v1 = faceVertices[0];
//
//            for (int j = 1; j < faceVertices.size() - 1; j++){
//                auto& v2 = faceVertices[j];
//                auto& v3 = faceVertices[j + 1];
//
//                state.addFace(
//                        parseInt(v1[0]), parseInt(v2[0]), parseInt(v3[0]),
//                        v1.size() > 1 ? parseInt(v1[1]) : numeric_limits<int>::quiet_NaN(), v2.size() > 1 ? parseInt(v2[1]) : numeric_limits<int>::quiet_NaN(), v3.size() > 1 ? parseInt(v3[1]) : numeric_limits<int>::quiet_NaN(),
//                        v1.size() > 2 ? parseInt(v1[2]) : numeric_limits<int>::quiet_NaN(), v2.size() > 2 ? parseInt(v2[2]) : numeric_limits<int>::quiet_NaN(), v3.size() > 2 ? parseInt(v3[2]) : numeric_limits<int>::quiet_NaN()
//                );
//            }
//        }
//        else if(lineFirstChar == 'l'){
//            auto lineParts = split(line, ' ');
//            lineParts.erase(lineParts.begin());
//            lineVertices.clear();
//            lineUVs.clear();
//
//            if (line.find("/") == -1){
//                lineVertices = lineParts;
//            }else{
//                for (int li = 0; li < lineParts.size(); li++){
//                    auto parts = split(lineParts[li], '/');
//
//                    if (parts[0] != "") lineVertices.push_back(parts[0]);
//                    if (parts[1] != "") lineUVs.push_back(parts[1]);
//                }
//            }
//            state.addLineGeometry(lineVertices, lineUVs);
//
//        }
//        else if(lineFirstChar == 'p'){
//            auto lineData = line.substr(1);
//            auto pointData = split(lineData, ' ');
//
//            state.addPointGeometry(pointData);
//        }
//        else if(lineFirstChar == 's'){
//            auto result = split(line, ' ');
//            if (result.size() > 1){
//                toLowerCase(result[1]);
//                state.object->smooth = (result[1] != "0" && result[1] != "off");
//            }else{
//                state.object->smooth = true;
//            }
//            auto& material = state.object->currentMaterial();
//            if (material->index != -1) material->smooth = state.object->smooth;
//            continue;
//        }
//        else if(lineFirstChar == 'o' || lineFirstChar == 'g'){
//            auto name = line.substr(2);
//            state.startObject(name, true);
//            continue;
//        }
//        else if (lineFirstChar == 'u') // usemtl
//        {
//            string mtlName = line.substr(7);
//            state.object->startMaterial(mtlName, state.materialLibraries);
//            continue;
//        } else if (lineFirstChar == 'm') // mtllib
//        {
//            string matFileName = line.substr(7);
//            string directory = filePath;
//            string matPath = directory + threecpp::getFileSeparator() + matFileName;
//            //state.Object.StartMaterial(matPath, state.MaterialLibraries);
//            state.materialLibraries.push_back(matPath);
//            continue;
//        }else{
//            if (line == "\0")
//                continue;
//            else {
//                throw std::runtime_error("THREE.OBJLoader: Unexpected line:" + line);
//            }
//        }
    }

    state.finalize();

    Group::sptr container = Group::create();

    container->materialLibraries = state.materialLibraries;

    bool hasPrimitives = !(state.objects.size() == 1 && state.objects[0]->geometry.vertices.size() == 0);

    if(hasPrimitives) {
        if (container->materialLibraries.size() > 0) {
            MTLLoader mtlLoader;
            for (int i = 0; i < container->materialLibraries.size(); i++) {
                string mtlPath = container->materialLibraries[i];
                if (threecpp::FileExists(mtlPath))
                    mtlLoader.load(mtlPath);
            }
            //mtlLoader.multiMaterialCreator->filePath = threecpp::getFileDir(filePath);
            if(mtlLoader.multiMaterialCreator!= nullptr)
                setMaterials(*mtlLoader.multiMaterialCreator);
        }

        for (size_t i = 0; i < state.objects.size(); i++) {
            auto &object = state.objects[i];
            auto &geometry = object->geometry;
            auto &materials = object->materials;
            bool isLine = geometry.type == "Line" ? true : false;
            bool isPoints = geometry.type == "Points" ? true : false;
            bool hasVertexColors = false;

            // Skip o/g line declarations that did not follow with any faces
            if (geometry.vertices.size() == 0) continue;

            auto bufferGeometry = BufferGeometry::create();

            bufferGeometry->setAttribute(AttributeName::position, BufferAttribute<float>::create(geometry.vertices, 3));

            if (geometry.normals.size() > 0) {
                bufferGeometry->setAttribute(AttributeName::normal,
                                             BufferAttribute<float>::create(geometry.normals, 3));
            } else {
                bufferGeometry->computeVertexNormals();
            }

            if (geometry.colors.size() > 0) {
                hasVertexColors = true;
                bufferGeometry->setAttribute(AttributeName::color, BufferAttribute<float>::create(geometry.colors, 3));
            }

            if (geometry.hasUVIndices) {
                bufferGeometry->setAttribute(AttributeName::uv, BufferAttribute<float>::create(geometry.uvs, 2));
            }

            // Create materials
            vector<Material::sptr> createdMaterials;

            for (int mi = 0; mi < materials.size(); mi++) {

                auto sourceMaterial = materials[mi];
                Material::sptr material = nullptr;

                if (materials.size() > 0 && !sourceMaterial->name.empty()) {

                    material = this->materials.create(sourceMaterial->name);

                    // mtl etc. loaders probably can't create line materials correctly, copy properties to a line material.
                    if (isLine && material != nullptr && !instanceOf<LineBasicMaterial>(material.get())) {

                        LineBasicMaterial::sptr materialLine = LineBasicMaterial::create();
                        materialLine->copy(*material);
                        material = materialLine;

                    } else if (isPoints && material != nullptr && !instanceOf<PointsMaterial>(material.get())) {

                        PointsMaterial::sptr materialPoints = PointsMaterial::create();
                        materialPoints->copy(*std::dynamic_pointer_cast<PointsMaterial>(material));
                        materialPoints->size = 10;
                        materialPoints->sizeAttenuation = false;
                        materialPoints->map = material->map;
                        material = materialPoints;
                    }

                }

                if (material == nullptr) {
                    if (isLine) {
                        material = LineBasicMaterial::create();
                    } else if (isPoints) {
                        material = PointsMaterial::create();
                        material->size = 1;
                        material->sizeAttenuation = false;
                    } else {
                        material = MeshPhongMaterial::create();
                    }

                    material->name = sourceMaterial->name;
                }

                material->flatShading = sourceMaterial->smooth ? false : true;
                material->vertexColors = hasVertexColors ? true : false;

                createdMaterials.push_back(material);
            }

            // Create Mesh
            Mesh::sptr mesh;

            if (createdMaterials.size() > 1) {

                for (int mi = 0; mi < materials.size(); mi++) {

                    auto sourceMaterial = materials[mi];
                    bufferGeometry->addGroup(sourceMaterial->groupStart, sourceMaterial->groupCount, mi);

                }

                if (isLine) {
                    auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials);
                    lineSegment->name = object->name;
                    container->add(lineSegment);
                } else if (isPoints) {

                    auto points = Points::create(bufferGeometry, createdMaterials);
                    points->name = object->name;
                    container->add(points);

                } else {
                    mesh = Mesh::create(bufferGeometry, createdMaterials);
                    mesh->name = object->name;
                    container->add(mesh);
                }

            } else {
                if (isLine) {
                    auto lineSegment = LineSegments::create(bufferGeometry, createdMaterials[0]);
                    lineSegment->name = object->name;
                    container->add(lineSegment);
                } else if (isPoints) {
                    auto points = Points::create(bufferGeometry, createdMaterials[0]);
                    points->name = object->name;
                    container->add(points);
                } else {
                    mesh = Mesh::create(bufferGeometry, createdMaterials[0]);
                    mesh->name = object->name;
                    container->add(mesh);
                }
            }
        }
    }else{
        // if there is only the default parser state object with no geometry data, interpret data as point cloud
        if ( state.vertices.size() > 0 ) {

            PointsMaterial::sptr material = PointsMaterial::create();
            material->size = 1.0;
            material->sizeAttenuation = false;

            BufferGeometry::sptr buffergeometry = BufferGeometry::create();

            buffergeometry->setAttribute(AttributeName::position, BufferAttribute<float>::create( state.vertices, 3 ) );

            if ( state.colors.size() > 0/** && state.colors[ 0 ] !=  */) {
                buffergeometry->setAttribute( AttributeName::color, BufferAttribute<float>::create( state.colors, 3 ) );
                material->vertexColors = true;
            }

            Points::sptr points = Points::create( buffergeometry, material );
            container->add( points );

        }
    }

    return container;
}

Group::sptr OBJLoader::load(const string& path){
    return parse(path);
}

