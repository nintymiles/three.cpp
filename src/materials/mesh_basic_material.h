//
// Created by SeanR on 2022/11/14.
//

#ifndef THREE_CPP_SRC_MATERIALS_MESH_BASIC_MATERIAL_H
#define THREE_CPP_SRC_MATERIALS_MESH_BASIC_MATERIAL_H

#include "material.h"
#include "color.h"
#include "constants.h"

#include <string>
#include <map>

class MeshBasicMaterial:public Material{
public:
    bool isMeshBasicMaterial = true;
    std::string type = "MeshBasicMaterial";
    Color color{0xffffff}; //emissive
    //std::map

//    this.map = null;

//    this.lightMap = null;
    double lightMapIntensity = 1.0;

//    this.aoMap = null;
    double aoMapIntensity = 1.0;

//    this.specularMap = null;

//    this.alphaMap = null;

//    this.envMap = null;
    int combine = MultiplyOperation;
    double reflectivity = 1;
    double refractionRatio = 0.98;

    bool wireframe = false;
    double wireframeLinewidth = 1;
    std::string wireframeLinecap = "round";
    std::string wireframeLinejoin = "round";


};

//import { Material } from './Material.js';
//import { MultiplyOperation } from '../constants.js';
//import { Color } from '../math/Color.js';
//
//class MeshBasicMaterial extends Material {
//
//constructor( parameters ) {
//
//    super();
//
//    this.type = 'MeshBasicMaterial';
//
//    this.color = new Color( 0xffffff ); // emissive
//
//    this.map = null;
//
//    this.lightMap = null;
//    this.lightMapIntensity = 1.0;
//
//    this.aoMap = null;
//    this.aoMapIntensity = 1.0;
//
//    this.specularMap = null;
//
//    this.alphaMap = null;
//
//    this.envMap = null;
//    this.combine = MultiplyOperation;
//    this.reflectivity = 1;
//    this.refractionRatio = 0.98;
//
//    this.wireframe = false;
//    this.wireframeLinewidth = 1;
//    this.wireframeLinecap = 'round';
//    this.wireframeLinejoin = 'round';
//
//    this.setValues( parameters );
//
//}
//
//copy( source ) {
//
//    super.copy( source );
//
//    this.color.copy( source.color );
//
//    this.map = source.map;
//
//    this.lightMap = source.lightMap;
//    this.lightMapIntensity = source.lightMapIntensity;
//
//    this.aoMap = source.aoMap;
//    this.aoMapIntensity = source.aoMapIntensity;
//
//    this.specularMap = source.specularMap;
//
//    this.alphaMap = source.alphaMap;
//
//    this.envMap = source.envMap;
//    this.combine = source.combine;
//    this.reflectivity = source.reflectivity;
//    this.refractionRatio = source.refractionRatio;
//
//    this.wireframe = source.wireframe;
//    this.wireframeLinewidth = source.wireframeLinewidth;
//    this.wireframeLinecap = source.wireframeLinecap;
//    this.wireframeLinejoin = source.wireframeLinejoin;
//
//    return this;
//
//}
//
//}
//
//MeshBasicMaterial.prototype.isMeshBasicMaterial = true;
//
//export { MeshBasicMaterial };


#endif //THREE_CPP_SRC_MATERIALS_MESH_BASIC_MATERIAL_H
