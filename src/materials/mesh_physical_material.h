//
// Created by SeanR on 2026/3/22.
//

#ifndef THREE_CPP_MESH_PHYSICAL_MATERIAL_H
#define THREE_CPP_MESH_PHYSICAL_MATERIAL_H

#include "mesh_standard_material.h"
#include "math_utils.h"

class MeshPhysicalMaterial : public MeshStandardMaterial{
public:
    using sptr = std::shared_ptr<MeshPhysicalMaterial>;
    float ior = 0.f;
    Texture::sptr iridescenceMap = nullptr;
    float iridescenceIOR = 0.f;
    std::pair<float,float> iridescenceThicknessRange;
    Texture::sptr iridescenceThicknessMap;
    Color sheenColor;
    Texture::sptr sheenColorMap;
    float sheenRoughness;
    Texture::sptr sheenRoughnessMap;

    Texture::sptr transmissionMap;

    float thickness;
    Texture::sptr thicknessMap;
    float attenuationDistance;
    Color attenuationColor;

    float specularIntensity;
    Texture::sptr specularIntensityMap;
    Color specularColor;
    Texture::sptr specularColorMap;


    MeshPhysicalMaterial() :MeshStandardMaterial(){
        definesString.insert({"STANDARD", std::string()});
        definesString.insert({"PHYSICAL", std::string()});
        type = "MeshPhysicalMaterial";
        name = "MeshPhysicalMaterial";

        shaderId = "physical";

        clearcoatMap = nullptr;
        clearcoatRoughness = 0.0;
        clearcoatRoughnessMap = nullptr;
        clearcoatNormalScale = Vector2( 1, 1 );
        clearcoatNormalMap = nullptr;

        ior = 1.5;

        iridescenceMap = nullptr;
        iridescenceIOR = 1.3;
        iridescenceThicknessRange = {100, 400};
        iridescenceThicknessMap = nullptr;

        sheenColor = Color( 0x000000 );
        sheenColorMap = nullptr;
        sheenRoughness = 1.0;
        sheenRoughnessMap = nullptr;

        transmissionMap = nullptr;

        thickness = 0;
        thicknessMap = nullptr;
        attenuationDistance = INFINITY;
        attenuationColor = Color( 1, 1, 1 );

        specularIntensity = 1.0;
        specularIntensityMap = nullptr;
        specularColor = Color( 1, 1, 1 );
        specularColorMap = nullptr;

        _sheen = 0.0;
        _clearcoat = 0;
        _iridescence = 0;
        _transmission = 0;

    }
    MeshPhysicalMaterial& setReflectivity(float reflectivity){
        this->ior = ( 1 + 0.4 * reflectivity ) / ( 1 - 0.4 * reflectivity );
        return *this;
    }
    float getReflectivity(){
        return clamp( 2.5 * ( this->ior - 1 ) / ( this->ior + 1 ), 0., 1. );
    }

    float sheen() const{
        return this->_sheen;
    }

    MeshPhysicalMaterial& sheen(float value){
        if ( this->_sheen > 0 != value > 0 ) {
            this->version ++;
        }
        this->_sheen = value;
        return *this;
    }

    float clearcoat() const{
        return this->_clearcoat;
    }

    MeshPhysicalMaterial& clearcoat( float value ) {
        if ( this->_clearcoat > 0 != value > 0 ) {
            this->version ++;
        }

        this->_clearcoat = value;
        return *this;
    }

    float iridescence() const{
        return this->_iridescence;
    }

    MeshPhysicalMaterial& iridescence( float value ) {
        if ( this->_iridescence > 0 != value > 0 ) {
            this->version ++;
        }

        this->_iridescence = value;
        return *this;
    }

    float transmission() const{
        return this->_transmission;
    }

    MeshPhysicalMaterial& transmission( float value ) {
        if ( this->_transmission > 0 != value > 0 ) {
            this->version ++;
        }

        this->_transmission = value;
        return *this;
    }


    MeshPhysicalMaterial(const MeshPhysicalMaterial& source) : MeshStandardMaterial(source) {
        definesString.insert({"STANDARD", std::string()});
        definesString.insert({"PHYSICAL", std::string()});
        type = source.type;
        name = source.name;

        shaderId = source.shaderId;

        clearcoatMap = source.clearcoatMap;
        clearcoatRoughness = source.clearcoatRoughness;
        clearcoatRoughnessMap = source.clearcoatRoughnessMap;
        clearcoatNormalScale = source.clearcoatNormalScale;
        clearcoatNormalMap = source.clearcoatNormalMap;

        ior = source.ior;

        _iridescence = source.iridescence();
        iridescenceMap = source.iridescenceMap;
        iridescenceIOR = source.iridescenceIOR;
        iridescenceThicknessRange = source.iridescenceThicknessRange;
        iridescenceThicknessMap = source.iridescenceThicknessMap;

        _sheen = source.sheen();
        sheenColor.copy( source.sheenColor );
        sheenColorMap = source.sheenColorMap;
        sheenRoughness = source.sheenRoughness;
        sheenRoughnessMap = source.sheenRoughnessMap;

        _transmission = source.transmission();
        transmissionMap = source.transmissionMap;

        thickness = source.thickness;
        thicknessMap = source.thicknessMap;
        attenuationDistance = source.attenuationDistance;
        attenuationColor = source.attenuationColor;

        specularIntensity = source.specularIntensity;
        specularIntensityMap = source.specularIntensityMap;
        specularColor = source.specularColor;
        specularColorMap = source.specularColorMap;
    }

    static sptr create() {
        return std::make_shared<MeshPhysicalMaterial>();
    }

    virtual ~MeshPhysicalMaterial() = default;

    virtual MeshPhysicalMaterial* clone(){
        return new MeshPhysicalMaterial(*this);
    }

    virtual MeshPhysicalMaterial& copy(const MeshPhysicalMaterial& source){
        MeshStandardMaterial::copy(source);

        definesString.insert({"STANDARD", std::string()});
        definesString.insert({"PHYSICAL", std::string()});

        type = source.type;
        name = source.name;

        shaderId = source.shaderId;

        clearcoatMap = source.clearcoatMap;
        clearcoatRoughness = source.clearcoatRoughness;
        clearcoatRoughnessMap = source.clearcoatRoughnessMap;
        clearcoatNormalScale = source.clearcoatNormalScale;
        clearcoatNormalMap = source.clearcoatNormalMap;

        ior = source.ior;

        _iridescence = source.iridescence();
        iridescenceMap = source.iridescenceMap;
        iridescenceIOR = source.iridescenceIOR;
        iridescenceThicknessRange = source.iridescenceThicknessRange;
        iridescenceThicknessMap = source.iridescenceThicknessMap;

        _sheen = source.sheen();
        sheenColor.copy( source.sheenColor );
        sheenColorMap = source.sheenColorMap;
        sheenRoughness = source.sheenRoughness;
        sheenRoughnessMap = source.sheenRoughnessMap;

        _transmission = source.transmission();
        transmissionMap = source.transmissionMap;

        thickness = source.thickness;
        thicknessMap = source.thicknessMap;
        attenuationDistance = source.attenuationDistance;
        attenuationColor = source.attenuationColor;

        specularIntensity = source.specularIntensity;
        specularIntensityMap = source.specularIntensityMap;
        specularColor = source.specularColor;
        specularColorMap = source.specularColorMap;

        return *this;
    }

private:
    float _sheen = 0.0;
    float _clearcoat = 0;
    float _iridescence = 0;
    float _transmission = 0;
};

#endif //THREE_CPP_MESH_PHYSICAL_MATERIAL_H
