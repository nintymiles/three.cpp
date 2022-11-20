//
// Created by SeanR on 2022/10/14.
//

#ifndef THREE_CPP_SRC_MATERIAL_H
#define THREE_CPP_SRC_MATERIAL_H

#include "event_dispatcher.h"
#include "constants.h"
#include "math_utils.h"
#include "plane.h"

#include <map>
#include <string>

int materialId = 0;

class Material:public EventDispatcher{
public:
    int id = materialId++;
    std::string uuid = MathUtils::generate_uuid();
    std::string name = "";
    std::string type = "Material";
    bool isMaterial = true;
    bool fog = true;

    bool blending = NormalBlending;
    bool side = FrontSide;
    bool vertexColors = false;

    int opacity = 1;
    bool transparent = false;

    int blendSrc = SrcAlphaFactor;
    int blendDst = OneMinusSrcAlphaFactor;
    int blendEquation = AddEquation;
    bool blendSrcAlpha ;
    bool blendDstAlpha ;
    bool blendEquationAlpha ;

    int depthFunc = LessEqualDepth;
    bool depthTest = true;
    bool depthWrite = true;

    int stencilWriteMask = 0xff;
    int stencilFunc = AlwaysStencilFunc;
    int stencilRef = 0;
    int stencilFuncMask = 0xff;
    int stencilFail = KeepStencilOp;
    int stencilZFail = KeepStencilOp;
    int stencilZPass = KeepStencilOp;
    bool stencilWrite = false;

    std::vector<Plane> clippingPlanes;
    bool clipIntersection = false;
    bool clipShadows = false;

    int shadowSide;

    bool colorWrite = true;

    int precision; // override the renderer's default precision for this material

    bool polygonOffset = false;
    int polygonOffsetFactor = 0;
    int polygonOffsetUnits = 0;

    bool dithering = false;

    bool alphaToCoverage = false;
    bool premultipliedAlpha = false;

    bool visible = true;

    bool toneMapped = true;

    std::map<std::string,int> userData = {};

    int version = 0;

    int _alphaTest = 0;

    Material()=default;

    int alphaTest(){
        return _alphaTest;
    }

    Material& alphaTest( int value ) {
        if ( _alphaTest > 0 != value > 0 ) {
            version ++;
        }

        _alphaTest = value;

        return *this;
    }

    virtual Material& onBuild( /* shaderobject, renderer */ ) {}

    virtual Material& onBeforeRender( /* renderer, scene, camera, geometry, object, group */ ) {}

    virtual Material& onBeforeCompile( /* shaderobject, renderer */ ) {}

//    customProgramCacheKey() {
//        return this.onBeforeCompile.toString();
//    }

//    setValues( values ) {
//
//        if ( values === undefined ) return;
//
//        for ( const key in values ) {
//
//            const newValue = values[ key ];
//
//            if ( newValue === undefined ) {
//
//                console.warn( 'THREE.Material: \'' + key + '\' parameter is undefined.' );
//                continue;
//
//            }
//
//            // for backward compatibility if shading is set in the constructor
//            if ( key === 'shading' ) {
//
//                console.warn( 'THREE.' + this.type + ': .shading has been removed. Use the boolean .flatShading instead.' );
//                this.flatShading = ( newValue === FlatShading ) ? true : false;
//                continue;
//
//            }
//
//            const currentValue = this[ key ];
//
//            if ( currentValue === undefined ) {
//
//                console.warn( 'THREE.' + this.type + ': \'' + key + '\' is not a property of this material.' );
//                continue;
//
//            }
//
//            if ( currentValue && currentValue.isColor ) {
//
//                currentValue.set( newValue );
//
//            } else if ( ( currentValue && currentValue.isVector3 ) && ( newValue && newValue.isVector3 ) ) {
//
//                currentValue.copy( newValue );
//
//            } else {
//
//                this[ key ] = newValue;
//
//            }
//
//        }

    Material clone() {
        return *this;
    }

    Material(const Material& m):name(m.name),
            fog(m.fog),blending(m.blending),side(m.side),vertexColors(m.vertexColors),
            opacity(m.opacity),transparent(m.transparent),
            blendSrc(m.blendSrc),blendDst(m.blendDst),blendEquation(m.blendEquation),blendSrcAlpha(m.blendSrcAlpha),
            blendDstAlpha(m.blendDstAlpha),blendEquationAlpha(m.blendEquationAlpha),
            depthFunc(m.depthFunc),depthTest(m.depthTest),depthWrite(m.depthWrite),
            stencilWriteMask(m.stencilWriteMask),stencilFunc(m.stencilFunc),
            stencilRef(m.stencilRef),stencilFuncMask(m.stencilFuncMask),
            stencilFail(m.stencilFail),stencilZFail(m.stencilZFail),
            stencilZPass(m.stencilZPass),stencilWrite(m.stencilWrite),
            clipIntersection(m.clipIntersection),clipShadows(m.clipShadows),
            shadowSide(m.shadowSide),colorWrite(m.colorWrite),precision(m.precision),
            polygonOffset(m.polygonOffset),polygonOffsetFactor(m.polygonOffsetFactor),polygonOffsetUnits(m.polygonOffsetUnits),
            dithering(m.dithering),alphaToCoverage(m.alphaToCoverage),
            premultipliedAlpha(m.premultipliedAlpha),visible(m.visible),
            toneMapped(m.toneMapped),clippingPlanes(m.clippingPlanes),_alphaTest(m._alphaTest){}


//        this.alphaTest = source.alphaTest;
//        this.userData = JSON.parse( JSON.stringify( source.userData ) );
//        return this;

    Material& dispose() {
        dispatchEvent( { type: "dispose" } );

        return *this;
    }

    Material& needsUpdate( bool value ) {
        if ( value == true ) version ++;

        return *this;
    }

//    Material& fromType(){
//        // TODO: Behavior added in Materials.js
//        return null;
//    }


};

//toJSON( meta ) {
//
//    const isRootObject = ( meta === undefined || typeof meta === 'string' );
//
//    if ( isRootObject ) {
//
//        meta = {
//            textures: {},
//            images: {}
//        };
//
//    }
//
//    const data = {
//        metadata: {
//        version: 4.5,
//        type: 'Material',
//            generator: 'Material.toJSON'
//    }
//    };
//
//    // standard Material serialization
//    data.uuid = this.uuid;
//    data.type = this.type;
//
//    if ( this.name !== '' ) data.name = this.name;
//
//    if ( this.color && this.color.isColor ) data.color = this.color.getHex();
//
//    if ( this.roughness !== undefined ) data.roughness = this.roughness;
//    if ( this.metalness !== undefined ) data.metalness = this.metalness;
//
//    if ( this.sheen !== undefined ) data.sheen = this.sheen;
//    if ( this.sheenColor && this.sheenColor.isColor ) data.sheenColor = this.sheenColor.getHex();
//    if ( this.sheenRoughness !== undefined ) data.sheenRoughness = this.sheenRoughness;
//    if ( this.emissive && this.emissive.isColor ) data.emissive = this.emissive.getHex();
//    if ( this.emissiveIntensity && this.emissiveIntensity !== 1 ) data.emissiveIntensity = this.emissiveIntensity;
//
//    if ( this.specular && this.specular.isColor ) data.specular = this.specular.getHex();
//    if ( this.specularIntensity !== undefined ) data.specularIntensity = this.specularIntensity;
//    if ( this.specularColor && this.specularColor.isColor ) data.specularColor = this.specularColor.getHex();
//    if ( this.shininess !== undefined ) data.shininess = this.shininess;
//    if ( this.clearcoat !== undefined ) data.clearcoat = this.clearcoat;
//    if ( this.clearcoatRoughness !== undefined ) data.clearcoatRoughness = this.clearcoatRoughness;
//
//    if ( this.clearcoatMap && this.clearcoatMap.isTexture ) {
//
//        data.clearcoatMap = this.clearcoatMap.toJSON( meta ).uuid;
//
//    }
//
//    if ( this.clearcoatRoughnessMap && this.clearcoatRoughnessMap.isTexture ) {
//
//        data.clearcoatRoughnessMap = this.clearcoatRoughnessMap.toJSON( meta ).uuid;
//
//    }
//
//    if ( this.clearcoatNormalMap && this.clearcoatNormalMap.isTexture ) {
//
//        data.clearcoatNormalMap = this.clearcoatNormalMap.toJSON( meta ).uuid;
//        data.clearcoatNormalScale = this.clearcoatNormalScale.toArray();
//
//    }
//
//    if ( this.map && this.map.isTexture ) data.map = this.map.toJSON( meta ).uuid;
//    if ( this.matcap && this.matcap.isTexture ) data.matcap = this.matcap.toJSON( meta ).uuid;
//    if ( this.alphaMap && this.alphaMap.isTexture ) data.alphaMap = this.alphaMap.toJSON( meta ).uuid;
//
//    if ( this.lightMap && this.lightMap.isTexture ) {
//
//        data.lightMap = this.lightMap.toJSON( meta ).uuid;
//        data.lightMapIntensity = this.lightMapIntensity;
//
//    }
//
//    if ( this.aoMap && this.aoMap.isTexture ) {
//
//        data.aoMap = this.aoMap.toJSON( meta ).uuid;
//        data.aoMapIntensity = this.aoMapIntensity;
//
//    }
//
//    if ( this.bumpMap && this.bumpMap.isTexture ) {
//
//        data.bumpMap = this.bumpMap.toJSON( meta ).uuid;
//        data.bumpScale = this.bumpScale;
//
//    }
//
//    if ( this.normalMap && this.normalMap.isTexture ) {
//
//        data.normalMap = this.normalMap.toJSON( meta ).uuid;
//        data.normalMapType = this.normalMapType;
//        data.normalScale = this.normalScale.toArray();
//
//    }
//
//    if ( this.displacementMap && this.displacementMap.isTexture ) {
//
//        data.displacementMap = this.displacementMap.toJSON( meta ).uuid;
//        data.displacementScale = this.displacementScale;
//        data.displacementBias = this.displacementBias;
//
//    }
//
//    if ( this.roughnessMap && this.roughnessMap.isTexture ) data.roughnessMap = this.roughnessMap.toJSON( meta ).uuid;
//    if ( this.metalnessMap && this.metalnessMap.isTexture ) data.metalnessMap = this.metalnessMap.toJSON( meta ).uuid;
//
//    if ( this.emissiveMap && this.emissiveMap.isTexture ) data.emissiveMap = this.emissiveMap.toJSON( meta ).uuid;
//    if ( this.specularMap && this.specularMap.isTexture ) data.specularMap = this.specularMap.toJSON( meta ).uuid;
//    if ( this.specularIntensityMap && this.specularIntensityMap.isTexture ) data.specularIntensityMap = this.specularIntensityMap.toJSON( meta ).uuid;
//    if ( this.specularColorMap && this.specularColorMap.isTexture ) data.specularColorMap = this.specularColorMap.toJSON( meta ).uuid;
//
//    if ( this.envMap && this.envMap.isTexture ) {
//
//        data.envMap = this.envMap.toJSON( meta ).uuid;
//
//        if ( this.combine !== undefined ) data.combine = this.combine;
//
//    }
//
//    if ( this.envMapIntensity !== undefined ) data.envMapIntensity = this.envMapIntensity;
//    if ( this.reflectivity !== undefined ) data.reflectivity = this.reflectivity;
//    if ( this.refractionRatio !== undefined ) data.refractionRatio = this.refractionRatio;
//
//    if ( this.gradientMap && this.gradientMap.isTexture ) {
//
//        data.gradientMap = this.gradientMap.toJSON( meta ).uuid;
//
//    }
//
//    if ( this.transmission !== undefined ) data.transmission = this.transmission;
//    if ( this.transmissionMap && this.transmissionMap.isTexture ) data.transmissionMap = this.transmissionMap.toJSON( meta ).uuid;
//    if ( this.thickness !== undefined ) data.thickness = this.thickness;
//    if ( this.thicknessMap && this.thicknessMap.isTexture ) data.thicknessMap = this.thicknessMap.toJSON( meta ).uuid;
//    if ( this.attenuationDistance !== undefined ) data.attenuationDistance = this.attenuationDistance;
//    if ( this.attenuationColor !== undefined ) data.attenuationColor = this.attenuationColor.getHex();
//
//    if ( this.size !== undefined ) data.size = this.size;
//    if ( this.shadowSide !== null ) data.shadowSide = this.shadowSide;
//    if ( this.sizeAttenuation !== undefined ) data.sizeAttenuation = this.sizeAttenuation;
//
//    if ( this.blending !== NormalBlending ) data.blending = this.blending;
//    if ( this.side !== FrontSide ) data.side = this.side;
//    if ( this.vertexColors ) data.vertexColors = true;
//
//    if ( this.opacity < 1 ) data.opacity = this.opacity;
//    if ( this.transparent === true ) data.transparent = this.transparent;
//
//    data.depthFunc = this.depthFunc;
//    data.depthTest = this.depthTest;
//    data.depthWrite = this.depthWrite;
//    data.colorWrite = this.colorWrite;
//
//    data.stencilWrite = this.stencilWrite;
//    data.stencilWriteMask = this.stencilWriteMask;
//    data.stencilFunc = this.stencilFunc;
//    data.stencilRef = this.stencilRef;
//    data.stencilFuncMask = this.stencilFuncMask;
//    data.stencilFail = this.stencilFail;
//    data.stencilZFail = this.stencilZFail;
//    data.stencilZPass = this.stencilZPass;
//
//    // rotation (SpriteMaterial)
//    if ( this.rotation !== undefined && this.rotation !== 0 ) data.rotation = this.rotation;
//
//    if ( this.polygonOffset === true ) data.polygonOffset = true;
//    if ( this.polygonOffsetFactor !== 0 ) data.polygonOffsetFactor = this.polygonOffsetFactor;
//    if ( this.polygonOffsetUnits !== 0 ) data.polygonOffsetUnits = this.polygonOffsetUnits;
//
//    if ( this.linewidth !== undefined && this.linewidth !== 1 ) data.linewidth = this.linewidth;
//    if ( this.dashSize !== undefined ) data.dashSize = this.dashSize;
//    if ( this.gapSize !== undefined ) data.gapSize = this.gapSize;
//    if ( this.scale !== undefined ) data.scale = this.scale;
//
//    if ( this.dithering === true ) data.dithering = true;
//
//    if ( this.alphaTest > 0 ) data.alphaTest = this.alphaTest;
//    if ( this.alphaToCoverage === true ) data.alphaToCoverage = this.alphaToCoverage;
//    if ( this.premultipliedAlpha === true ) data.premultipliedAlpha = this.premultipliedAlpha;
//
//    if ( this.wireframe === true ) data.wireframe = this.wireframe;
//    if ( this.wireframeLinewidth > 1 ) data.wireframeLinewidth = this.wireframeLinewidth;
//    if ( this.wireframeLinecap !== 'round' ) data.wireframeLinecap = this.wireframeLinecap;
//    if ( this.wireframeLinejoin !== 'round' ) data.wireframeLinejoin = this.wireframeLinejoin;
//
//    if ( this.flatShading === true ) data.flatShading = this.flatShading;
//
//    if ( this.visible === false ) data.visible = false;
//
//    if ( this.toneMapped === false ) data.toneMapped = false;
//
//    if ( JSON.stringify( this.userData ) !== '{}' ) data.userData = this.userData;
//
//    // TODO: Copied from Object3D.toJSON
//
//    function extractFromCache( cache ) {
//
//        const values = [];
//
//        for ( const key in cache ) {
//
//            const data = cache[ key ];
//            delete data.metadata;
//            values.push( data );
//
//        }
//
//        return values;
//
//    }
//
//    if ( isRootObject ) {
//
//        const textures = extractFromCache( meta.textures );
//        const images = extractFromCache( meta.images );
//
//        if ( textures.length > 0 ) data.textures = textures;
//        if ( images.length > 0 ) data.images = images;
//
//    }
//
//    return data;
//
//}



#endif //THREE_CPP_SRC_MATERIAL_H