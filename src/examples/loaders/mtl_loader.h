#ifndef MTL_LOADER_H
#define MTL_LOADER_H
#include <limits>
#include <vector>
#include <string>
#include <unordered_map>

#include <filesystem>

#include "color.h"
#include "vector2.h"
#include "material.h"
#include "gl_program.h"
#include "texture.h"
#include "constants.h"

//using namespace math;

class MTLLoader {
public:
    enum class MTLName : unsigned {
        kd,
        ka,
        ks,
        ke,
        map_kd,
        map_ks,
        map_ke,
        norm,
        map_bump,
        bump,
        map_d,
        ns,
        ni, //index of refraction
        d,
        tr,
        illum
    };
    enum class MTLMapType : unsigned {
        map,
        specularMap,
        emissiveMap,
        normalMap,
        bumpMap,
        alphaMap
    };
    struct MTLParameters {
        std::string name;
        Side side;
        Color color;
        Color specular;
        Color emissive;
        Texture::sptr map;
        Texture::sptr specularMap;
        Texture::sptr emissiveMap;
        Texture::sptr normalMap;
        Texture::sptr bumpMap;
        Texture::sptr alphaMap;
        float bumpScale=1.0f;
        float shininess=30.0f;
        float opacity=1.0f;
        bool transparent=false;
    };

    struct MaterialCreatorOptions{
//			Side side = Side::None;
//			Wrapping wrap = Wrapping::None;
//			bool normalizedRGB = false;
//			bool ignoreZeroRGBs = false;
//			bool invertTrpproperty = false;
        Side side;
        Wrapping wrap;
        bool normalizedRGB;
        bool ignoreZeroRGBs;
        bool invertTrpproperty;

        MaterialCreatorOptions(Side side = Side::None,Wrapping wrap = Wrapping::None,bool normalizedRGB = false,bool ignoreZeroRGBs = false,bool invertTrpproperty = false):
                side(side),wrap(wrap),normalizedRGB(normalizedRGB),ignoreZeroRGBs(ignoreZeroRGBs),invertTrpproperty(invertTrpproperty){}

        bool isEmpty() {
            return (side==Side::None)|| (wrap== Wrapping::None);
        }

    };

    struct MaterialInfo {
//        std::vector<float> Ks;
//        std::vector<float> Kd;
//        std::vector<float> Ke;
//        std::vector<float> Ka;
        float value[3];
        std::string stringValue;
//        std::string map_kd;
//        std::string map_ks;
//        std::string map_ke;
//        std::string norm;
//        std::string map_bump;
//        std::string bump;
//        std::string map_d;
//        int Ns = std::numeric_limits<int>::quiet_NaN();
//        int D = std::numeric_limits<int>::quiet_NaN();
//        int Tr = std::numeric_limits<int>::quiet_NaN();
    };

    struct TexParams {
        Vector2 scale;
        Vector2 offset;
        std::string url;
    };

    using MaterialsInfoMap = std::unordered_map<MTLName, MaterialInfo>;

    class MaterialCreator {
    public:
        using sptr = std::shared_ptr<MaterialCreator>;

        std::string filePath;

        MaterialCreatorOptions options;

        std::unordered_map<std::string, MaterialsInfoMap> materialsInfo;

        std::unordered_map<std::string, Material::sptr> materials;

        std::vector<Material::sptr> materialsArray;

        std::unordered_map<std::string, std::string> nameLookUp;

        Side side;

        Wrapping wrap;

        std::string crossOrigin = "anonymous";

        MaterialCreator() {}

        ~MaterialCreator() = default;

        MaterialCreator(std::string path, MaterialCreatorOptions options = MaterialCreatorOptions());

        MaterialCreator& setCrossOrigin(std::string value);

        void setMaterials(std::unordered_map<std::string, MaterialsInfoMap>& MaterialsInfoMap);

        std::unordered_map<std::string, MaterialsInfoMap> convert(std::unordered_map<std::string, MaterialsInfoMap>& materialsInfo);

        void setMapForType(MTLParameters& parameter, MTLMapType mapType, const std::string& value);

        TexParams getTextureParams(const std::string& value, MTLParameters& matParams);

        static MaterialCreator::sptr create(){
            return std::make_shared<MaterialCreator>();
        }
        Material::sptr create(const std::string& materialName);

        Material::sptr createMaterial(const std::string& materialName);

        Texture::sptr loadTexture(std::string filePath, TextureMapping mapping = TextureMapping::Unknown);
    };

    MaterialCreatorOptions materialOptions;

    std::string crossOrigin;

    std::string mtlFilePath;

    MaterialCreator::sptr multiMaterialCreator;

    MTLLoader() {}

    ~MTLLoader() = default;

    MaterialCreator::sptr parse(const std::string& filepath);

    MaterialCreator::sptr load(const std::string& filepath);
};

#endif