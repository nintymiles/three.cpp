//
// Created by SeanR on 2022/10/14.
//

#ifndef THREE_CPP_SRC_MATERIAL_H
#define THREE_CPP_SRC_MATERIAL_H

#include "event_dispatcher.h"
#include "constants.h"
#include "math_utils.h"
#include "plane.h"
#include "texture.h"
#include "simplesignal.h"

#include <map>
#include <string>
#include <typeinfo>

using namespace math;

class GLShader;
class GLUniforms;
class GLProgram;
class UniformValues;
class GLRenderer;

class Material {
    static size_t materialId;

public:
    struct ShaderMaterialExtension {
        bool derivatives = false;
        bool fragDepth = false;
        bool drawBuffers = false;
        bool shaderTextureLOD = false;
    };
    using sptr = std::shared_ptr<Material>;
    using UniformValuesPtr= std::shared_ptr<UniformValues>;

    Texture::sptr alphaMap = nullptr;

    std::string shaderId;
    /**
    * Sets the alpha value to be used when running an alpha test. Default is 0.
    */
    float alphaTest = 0;

    Texture::sptr aoMap=nullptr;

    float aoMapIntensity;

    /**
     * Blending destination. It's one of the blending mode constants defined in Three.js. Default is {@link OneMinusSrcAlphaFactor}.
     */
    BlendingDstFactor blendDst = BlendingDstFactor::OneMinusSrcAlphaFactor;

    /**
     * The tranparency of the .blendDst. Default is null.
     */
    BlendingDstFactor blendDstAlpha = BlendingDstFactor::None;

    /**
     * Blending equation to use when applying blending. It's one of the constants defined in Three.js. Default is {@link AddEquation}.
     */
    BlendingEquation blendEquation = BlendingEquation::AddEquation;

    /**
     * The tranparency of the .blendEquation. Default is null.
     */
    BlendingEquation blendEquationAlpha = BlendingEquation::None;

    /**
     * Which blending to use when displaying objects with this material. Default is {@link NormalBlending}.
     */
    Blending blending = Blending::NormalBlending;

    /**
     * Blending source. It's one of the blending mode constants defined in Three.js. Default is {@link SrcAlphaFactor}.
     */
    BlendingDstFactor blendSrc = BlendingDstFactor::SrcAlphaFactor;;

    /**
     * The tranparency of the .blendSrc. Default is null.
     */
    BlendingDstFactor blendSrcAlpha = BlendingDstFactor::None;

    Texture::sptr bumpMap = nullptr;

    float bumpScale;

    Combine combine;


    float clearcoat;

    Texture::sptr clearcoatMap = nullptr;

    float clearcoatRoughness;

    Texture::sptr clearcoatRoughnessMap = nullptr;

    Vector2 clearcoatNormalScale;

    Texture::sptr clearcoatNormalMap=nullptr;

    /**
     * Changes the behavior of clipping planes so that only their intersection is clipped, rather than their union. Default is false.
     */
    bool clipIntersection = false;

    /**
     * User-defined clipping planes specified as THREE.Plane objects in world space. These planes apply to the objects this material is attached to. Points in space whose signed distance to the plane is negative are clipped (not rendered). See the WebGL / clipping /intersection example. Default is null.
     */
    std::vector<Plane> clippingPlanes;

    /**
     * Defines whether to clip shadows according to the clipping planes specified on this material. Default is false.
     */
    bool clipShadows = false;

    /**
     * Material Color
     */

    Color color;
    /**
     * Whether to render the material's color. This can be used in conjunction with a mesh's .renderOrder property to create invisible objects that occlude other objects. Default is true.
     */
    bool colorWrite = true;

    float dashSize;

    /**
     * Custom defines to be injected into the shader. These are passed in form of an object literal, with key/value pairs. { MY_CUSTOM_DEFINE: '' , PI2: Math.PI * 2 }.
     * The pairs are defined in both vertex and fragment shaders. Default is undefined.
     */
    std::unordered_map<std::string, std::string> definesString;
    std::unordered_map<std::string, float> definesFloat;

    /**
     * Which depth function to use. Default is {@link LessEqualDepth}. See the depth mode constants for all possible values.
     */
    DepthModes depthFunc = DepthModes::LessDepth;

    /**
     * Whether to have depth test enabled when rendering this material. Default is true.
     */
    bool depthTest = true;

    /**
     * Whether rendering this material has any effect on the depth buffer. Default is true.
     * When drawing 2D overlays it can be useful to disable the depth writing in order to layer several things together without creating z-index artifacts.
     */
    bool depthWrite = true;

    Texture::sptr displacementMap = nullptr;

    DepthPackingStrategies depthPacking;

    float displacementScale;

    float displacementBias;

    Texture::sptr emissiveMap=nullptr;

    Color emissive;

    float emissiveIntensity;

    Texture::sptr envMap=nullptr;

    float envMapIntensity;
    /**
     * Whether the material is affected by fog. Default is true.
     */
    bool fog = true;

    float gapSize;

    /**
     * Unique number of this material instance.
     */
    unsigned id;

    Texture::sptr lightMap=nullptr;

    float lightMapIntensity;

    float linewidth=1;

    std::string linecap;

    std::string linejoin;

    Texture::sptr map=nullptr;

    Texture::sptr matcap=nullptr;

    float nearDistance;

    float farDistance;

    Texture::sptr normalMap=nullptr;

    NormalMapTypes normalMapType = NormalMapTypes::None;

    Vector2 normalScale;

    Texture::sptr gradientMap=nullptr;

    Vector3 referencePosition;

    float reflectivity;

    float refractionRatio;

    Texture::sptr roughnessMap = nullptr;

    Texture::sptr metalnessMap = nullptr;

    float roughness;

    float metalness;


    float scale;

    Texture::sptr specularMap = nullptr;

    Color specular;

    float shininess;

    Color sheen; // shared_ptr<Color>

    Texture::sptr transmissionMap = nullptr;

    float transparency;

    /**
   * Whether rendering this material has any effect on the stencil buffer. Default is *false*.
   */
    bool stencilWrite = false;

    unsigned stencilWriteMask = 0xff;

    unsigned stencilFuncMask = 0xff;
    /**
   * The stencil comparison function to use. Default is {@link AlwaysStencilFunc}. See stencil operation constants for all possible values.
   */
    StencilFunc stencilFunc = StencilFunc::AlwaysStencilFunc;

    /**
   * The value to use when performing stencil comparisons or stencil operations. Default is *0*.
   */
    unsigned stencilRef = 0;


    /**
   * Which stencil operation to perform when the comparison function returns false. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
   */
    StencilOp stencilFail = StencilOp::KeepStencilOp;

    /**
   * Which stencil operation to perform when the comparison function returns true but the depth test fails. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
   */
    StencilOp stencilZFail = StencilOp::KeepStencilOp;

    /**
   * Which stencil operation to perform when the comparison function returns true and the depth test passes. Default is {@link KeepStencilOp}. See the stencil operation constants for all possible values.
   */
    StencilOp stencilZPass = StencilOp::KeepStencilOp;

    /**
     * Used to check whether this or derived classes are materials. Default is true.
     * You should not change this, as it used internally for optimisation.
     */
    //readonly isMaterial : true;

    bool morphTargets=false;

    bool morphNormals=false;

    bool vertexTangents = false;
    /**
     * Material name. Default is an empty string.
     */
    std::string name;

    /**
     * Specifies that the material needs to be updated, WebGL wise. Set it to true if you made changes that need to be reflected in WebGL.
     * This property is automatically set to true when instancing a new material.
     */
    bool needsUpdate;

    /**
     * Opacity. Default is 1.
     */
    float opacity = 1;

    /**
     * Whether to use polygon offset. Default is false. This corresponds to the POLYGON_OFFSET_FILL WebGL feature.
     */
    bool polygonOffset = false;

    /**
     * Sets the polygon offset factor. Default is 0.
     */
    unsigned polygonOffsetFactor = 0.0f;

    /**
     * Sets the polygon offset units. Default is 0.
     */
    unsigned polygonOffsetUnits = 0.0f;

    /**
     * Override the renderer's default precision for this material. Can be "highp", "mediump" or "lowp". Defaults is null.
     */
    std::string precision;//: 'highp' | 'mediump' | 'lowp' | null;

    /**
     * Whether to premultiply the alpha (transparency) value. See WebGL / Materials / Transparency for an example of the difference. Default is false.
     */
    bool premultipliedAlpha=false;

    /**
     * Whether to apply dithering to the color to remove the appearance of banding. Default is false.
     */
    bool dithering = false;

    /**
     * Define whether the material is rendered with flat shading. Default is false.
     */
    bool flatShading = false;

    /**
     * Defines which of the face sides will be rendered - front, back or both.
     * Default is THREE.FrontSide. Other options are THREE.BackSide and THREE.DoubleSide.
     */
    Side side = Side::FrontSide;

    /**
     * Defines which of the face sides will cast shadows. Default is *null*.
     * If *null*, the value is opposite that of side, above.
     */
    Side shadowSide = Side::None;

    bool skinning = false;

    /**
     * Defines whether this material is tone mapped according to the renderer's toneMapping setting.
     * Default is true.
     */
    bool toneMapped = true;

    /**
     * Defines whether this material is transparent. This has an effect on rendering as transparent objects need special treatment and are rendered after non-transparent objects.
     * When set to true, the extent to which the material is transparent is controlled by setting it's .opacity property.
     * Default is false.
     */
    bool transparent=false;

    /**
     * Value is the string 'Material'. This shouldn't be changed, and can be used to find all objects of this type in a scene.
     */
    std::string type;

    /**
     * UUID of this material instance. This gets automatically assigned, so this shouldn't be edited.
     */
    sole::uuid uuid;

    /**
     * Defines whether vertex coloring is used. Default is false.
     */
    bool vertexColors = false;

    /**
     * Defines whether this material is visible. Default is true.
     */
    bool visible = true;

    /**
     * An object that can be used to store custom data about the Material. It should not hold references to functions as these will not be cloned.
     */
    //userData: any;
    bool wireframe = false;

    float wireframeLinewidth;

    std::string wireframeLinecap;

    std::string wireframeLinejoin;

    float size = 1;

    float rotation;

    bool sizeAttenuation = false;

    bool isShaderMaterial = false;

    bool isRawShaderMaterial = false;


    bool lights = true; // set to use scene lights

    bool clipping = false; // set to use user-defined clipping planes
    /**
     * This starts at 0 and counts how many times .needsUpdate is set to true.
     */
    int version;

    UniformValuesPtr uniforms;

    bool uniformsNeedUpdate = false;

    std::string vertexShader;/* =
		R"(void main() {"
		"   gl_Position = projectionMatrix*modelViewMatrix*vec4(position,1.0);\n"
		"})";*/
    std::string fragmentShader;/* =
		R"(void main() {"
		"   gl_FragColor = vec4(1.0,0.0,0.0,1.0);\n"
		"})";*/


    std::shared_ptr<GLProgram> program;

    int numSupportedMorphTargets = 0;

    int numSupportedMorphNormals = 0;

    GLSLVersion glslVersion = GLSLVersion::GLSL3;

    std::string indexOfAttributeName="";

    ShaderMaterialExtension extensions;

    std::string customProgramCacheKey = "";

    Material();

    Material(const Material& source);

    static sptr create() {
        return std::make_shared<Material>();
    }
    virtual ~Material() = default;
    /**
     * Return a new material with the same parameters as this material.
     */
    virtual Material* clone();

    /**
     * Copy the parameters from the passed material into this material.
     * @param material
     */
    virtual Material& copy(const Material& source);


    /**
     * An optional callback that is executed immediately before the shader program is compiled. This function is called with the shader source code as a parameter. Useful for the modification of built-in materials.
     * @param shader Source code of the shader
     * @param renderer WebGLRenderer Context that is initializing the material
     */
    //onBeforeCompile(shader : Shader, renderer : WebGLRenderer) : void;

    /**
     * Sets the properties based on the values.
     * @param values A container with parameters.
     */
    //setValues(values: MaterialParameters) : void;

    threecpp::Signal<void(const GLShader&,const GLRenderer&)> onBeforeCompile;

    threecpp::Signal<void(Material&)> onDispose;
    /**
     * This disposes the material. Texture::ptrs of a material don't get disposed. These needs to be disposed by {@link Texture::ptr}.
     */
    virtual void dispose() {
        onDispose.emitSignal(*this);
        onDispose.disconnectAll();
    }
};

//int materialId = 0;
//
//class Material:public EventDispatcher{
//public:
//    int id = materialId++;
//    std::string uuid = MathUtils::generate_uuid();
//    std::string name = "";
//    std::string type = "Material";
//    bool isMaterial = true;
//    bool fog = true;
//
//    bool blending = NormalBlending;
//    bool side = FrontSide;
//    bool vertexColors = false;
//
//    int opacity = 1;
//    bool transparent = false;
//
//    int blendSrc = SrcAlphaFactor;
//    int blendDst = OneMinusSrcAlphaFactor;
//    int blendEquation = AddEquation;
//    bool blendSrcAlpha ;
//    bool blendDstAlpha ;
//    bool blendEquationAlpha ;
//
//    int depthFunc = LessEqualDepth;
//    bool depthTest = true;
//    bool depthWrite = true;
//
//    int stencilWriteMask = 0xff;
//    int stencilFunc = AlwaysStencilFunc;
//    int stencilRef = 0;
//    int stencilFuncMask = 0xff;
//    int stencilFail = KeepStencilOp;
//    int stencilZFail = KeepStencilOp;
//    int stencilZPass = KeepStencilOp;
//    bool stencilWrite = false;
//
//    std::vector<Plane> clippingPlanes;
//    bool clipIntersection = false;
//    bool clipShadows = false;
//
//    int shadowSide;
//
//    bool colorWrite = true;
//
//    int precision; // override the renderer's default precision for this material
//
//    bool polygonOffset = false;
//    int polygonOffsetFactor = 0;
//    int polygonOffsetUnits = 0;
//
//    bool dithering = false;
//
//    bool alphaToCoverage = false;
//    bool premultipliedAlpha = false;
//
//    bool visible = true;
//
//    bool toneMapped = true;
//
//    std::map<std::string,int> userData = {};
//
//    int version = 0;
//
//    int _alphaTest = 0;
//
//    bool wireframe = false;
//
//    Material()=default;
//
//    int alphaTest(){
//        return _alphaTest;
//    }
//
//    Material& alphaTest( int value ) {
//        if ( _alphaTest > 0 != value > 0 ) {
//            version ++;
//        }
//
//        _alphaTest = value;
//
//        return *this;
//    }
//
//    virtual Material& onBuild( /* shaderobject, renderer */ ) {}
//
//    virtual Material& onBeforeRender( /* renderer, scene, camera, geometry, object, group */ ) {}
//
//    virtual Material& onBeforeCompile( /* shaderobject, renderer */ ) {}
//
//    //virtual std:string what_am_i(){return typeid(*this).name();}
//
////    customProgramCacheKey() {
////        return this.onBeforeCompile.toString();
////    }
//
////    setValues( values ) {
////
////        if ( values === undefined ) return;
////
////        for ( const key in values ) {
////
////            const newValue = values[ key ];
////
////            if ( newValue === undefined ) {
////
////                console.warn( 'THREE.Material: \'' + key + '\' parameter is undefined.' );
////                continue;
////
////            }
////
////            // for backward compatibility if shading is set in the constructor
////            if ( key === 'shading' ) {
////
////                console.warn( 'THREE.' + this.type + ': .shading has been removed. Use the boolean .flatShading instead.' );
////                this.flatShading = ( newValue === FlatShading ) ? true : false;
////                continue;
////
////            }
////
////            const currentValue = this[ key ];
////
////            if ( currentValue === undefined ) {
////
////                console.warn( 'THREE.' + this.type + ': \'' + key + '\' is not a property of this material.' );
////                continue;
////
////            }
////
////            if ( currentValue && currentValue.isColor ) {
////
////                currentValue.set( newValue );
////
////            } else if ( ( currentValue && currentValue.isVector3 ) && ( newValue && newValue.isVector3 ) ) {
////
////                currentValue.copy( newValue );
////
////            } else {
////
////                this[ key ] = newValue;
////
////            }
////
////        }
//
//    Material clone() {
//        return *this;
//    }
//
//    Material(const Material& m):name(m.name),
//            fog(m.fog),blending(m.blending),side(m.side),vertexColors(m.vertexColors),
//            opacity(m.opacity),transparent(m.transparent),
//            blendSrc(m.blendSrc),blendDst(m.blendDst),blendEquation(m.blendEquation),blendSrcAlpha(m.blendSrcAlpha),
//            blendDstAlpha(m.blendDstAlpha),blendEquationAlpha(m.blendEquationAlpha),
//            depthFunc(m.depthFunc),depthTest(m.depthTest),depthWrite(m.depthWrite),
//            stencilWriteMask(m.stencilWriteMask),stencilFunc(m.stencilFunc),
//            stencilRef(m.stencilRef),stencilFuncMask(m.stencilFuncMask),
//            stencilFail(m.stencilFail),stencilZFail(m.stencilZFail),
//            stencilZPass(m.stencilZPass),stencilWrite(m.stencilWrite),
//            clipIntersection(m.clipIntersection),clipShadows(m.clipShadows),
//            shadowSide(m.shadowSide),colorWrite(m.colorWrite),precision(m.precision),
//            polygonOffset(m.polygonOffset),polygonOffsetFactor(m.polygonOffsetFactor),polygonOffsetUnits(m.polygonOffsetUnits),
//            dithering(m.dithering),alphaToCoverage(m.alphaToCoverage),
//            premultipliedAlpha(m.premultipliedAlpha),visible(m.visible),
//            toneMapped(m.toneMapped),clippingPlanes(m.clippingPlanes),_alphaTest(m._alphaTest){}
//
//
////        this.alphaTest = source.alphaTest;
////        this.userData = JSON.parse( JSON.stringify( source.userData ) );
////        return this;
//
//    Material& dispose() {
//        dispatchEvent( { type: "dispose" } );
//
//        return *this;
//    }
//
//    Material& needsUpdate( bool value ) {
//        if ( value == true ) version ++;
//
//        return *this;
//    }
//
////    Material& fromType(){
////        // TODO: Behavior added in Materials.js
////        return null;
////    }
//
//
//};
//
////toJSON( meta ) {
////
////    const isRootObject = ( meta === undefined || typeof meta === 'string' );
////
////    if ( isRootObject ) {
////
////        meta = {
////            textures: {},
////            images: {}
////        };
////
////    }
////
////    const data = {
////        metadata: {
////        version: 4.5,
////        type: 'Material',
////            generator: 'Material.toJSON'
////    }
////    };
////
////    // standard Material serialization
////    data.uuid = this.uuid;
////    data.type = this.type;
////
////    if ( this.name !== '' ) data.name = this.name;
////
////    if ( this.color && this.color.isColor ) data.color = this.color.getHex();
////
////    if ( this.roughness !== undefined ) data.roughness = this.roughness;
////    if ( this.metalness !== undefined ) data.metalness = this.metalness;
////
////    if ( this.sheen !== undefined ) data.sheen = this.sheen;
////    if ( this.sheenColor && this.sheenColor.isColor ) data.sheenColor = this.sheenColor.getHex();
////    if ( this.sheenRoughness !== undefined ) data.sheenRoughness = this.sheenRoughness;
////    if ( this.emissive && this.emissive.isColor ) data.emissive = this.emissive.getHex();
////    if ( this.emissiveIntensity && this.emissiveIntensity !== 1 ) data.emissiveIntensity = this.emissiveIntensity;
////
////    if ( this.specular && this.specular.isColor ) data.specular = this.specular.getHex();
////    if ( this.specularIntensity !== undefined ) data.specularIntensity = this.specularIntensity;
////    if ( this.specularColor && this.specularColor.isColor ) data.specularColor = this.specularColor.getHex();
////    if ( this.shininess !== undefined ) data.shininess = this.shininess;
////    if ( this.clearcoat !== undefined ) data.clearcoat = this.clearcoat;
////    if ( this.clearcoatRoughness !== undefined ) data.clearcoatRoughness = this.clearcoatRoughness;
////
////    if ( this.clearcoatMap && this.clearcoatMap.isTexture ) {
////
////        data.clearcoatMap = this.clearcoatMap.toJSON( meta ).uuid;
////
////    }
////
////    if ( this.clearcoatRoughnessMap && this.clearcoatRoughnessMap.isTexture ) {
////
////        data.clearcoatRoughnessMap = this.clearcoatRoughnessMap.toJSON( meta ).uuid;
////
////    }
////
////    if ( this.clearcoatNormalMap && this.clearcoatNormalMap.isTexture ) {
////
////        data.clearcoatNormalMap = this.clearcoatNormalMap.toJSON( meta ).uuid;
////        data.clearcoatNormalScale = this.clearcoatNormalScale.toArray();
////
////    }
////
////    if ( this.map && this.map.isTexture ) data.map = this.map.toJSON( meta ).uuid;
////    if ( this.matcap && this.matcap.isTexture ) data.matcap = this.matcap.toJSON( meta ).uuid;
////    if ( this.alphaMap && this.alphaMap.isTexture ) data.alphaMap = this.alphaMap.toJSON( meta ).uuid;
////
////    if ( this.lightMap && this.lightMap.isTexture ) {
////
////        data.lightMap = this.lightMap.toJSON( meta ).uuid;
////        data.lightMapIntensity = this.lightMapIntensity;
////
////    }
////
////    if ( this.aoMap && this.aoMap.isTexture ) {
////
////        data.aoMap = this.aoMap.toJSON( meta ).uuid;
////        data.aoMapIntensity = this.aoMapIntensity;
////
////    }
////
////    if ( this.bumpMap && this.bumpMap.isTexture ) {
////
////        data.bumpMap = this.bumpMap.toJSON( meta ).uuid;
////        data.bumpScale = this.bumpScale;
////
////    }
////
////    if ( this.normalMap && this.normalMap.isTexture ) {
////
////        data.normalMap = this.normalMap.toJSON( meta ).uuid;
////        data.normalMapType = this.normalMapType;
////        data.normalScale = this.normalScale.toArray();
////
////    }
////
////    if ( this.displacementMap && this.displacementMap.isTexture ) {
////
////        data.displacementMap = this.displacementMap.toJSON( meta ).uuid;
////        data.displacementScale = this.displacementScale;
////        data.displacementBias = this.displacementBias;
////
////    }
////
////    if ( this.roughnessMap && this.roughnessMap.isTexture ) data.roughnessMap = this.roughnessMap.toJSON( meta ).uuid;
////    if ( this.metalnessMap && this.metalnessMap.isTexture ) data.metalnessMap = this.metalnessMap.toJSON( meta ).uuid;
////
////    if ( this.emissiveMap && this.emissiveMap.isTexture ) data.emissiveMap = this.emissiveMap.toJSON( meta ).uuid;
////    if ( this.specularMap && this.specularMap.isTexture ) data.specularMap = this.specularMap.toJSON( meta ).uuid;
////    if ( this.specularIntensityMap && this.specularIntensityMap.isTexture ) data.specularIntensityMap = this.specularIntensityMap.toJSON( meta ).uuid;
////    if ( this.specularColorMap && this.specularColorMap.isTexture ) data.specularColorMap = this.specularColorMap.toJSON( meta ).uuid;
////
////    if ( this.envMap && this.envMap.isTexture ) {
////
////        data.envMap = this.envMap.toJSON( meta ).uuid;
////
////        if ( this.combine !== undefined ) data.combine = this.combine;
////
////    }
////
////    if ( this.envMapIntensity !== undefined ) data.envMapIntensity = this.envMapIntensity;
////    if ( this.reflectivity !== undefined ) data.reflectivity = this.reflectivity;
////    if ( this.refractionRatio !== undefined ) data.refractionRatio = this.refractionRatio;
////
////    if ( this.gradientMap && this.gradientMap.isTexture ) {
////
////        data.gradientMap = this.gradientMap.toJSON( meta ).uuid;
////
////    }
////
////    if ( this.transmission !== undefined ) data.transmission = this.transmission;
////    if ( this.transmissionMap && this.transmissionMap.isTexture ) data.transmissionMap = this.transmissionMap.toJSON( meta ).uuid;
////    if ( this.thickness !== undefined ) data.thickness = this.thickness;
////    if ( this.thicknessMap && this.thicknessMap.isTexture ) data.thicknessMap = this.thicknessMap.toJSON( meta ).uuid;
////    if ( this.attenuationDistance !== undefined ) data.attenuationDistance = this.attenuationDistance;
////    if ( this.attenuationColor !== undefined ) data.attenuationColor = this.attenuationColor.getHex();
////
////    if ( this.size !== undefined ) data.size = this.size;
////    if ( this.shadowSide !== null ) data.shadowSide = this.shadowSide;
////    if ( this.sizeAttenuation !== undefined ) data.sizeAttenuation = this.sizeAttenuation;
////
////    if ( this.blending !== NormalBlending ) data.blending = this.blending;
////    if ( this.side !== FrontSide ) data.side = this.side;
////    if ( this.vertexColors ) data.vertexColors = true;
////
////    if ( this.opacity < 1 ) data.opacity = this.opacity;
////    if ( this.transparent === true ) data.transparent = this.transparent;
////
////    data.depthFunc = this.depthFunc;
////    data.depthTest = this.depthTest;
////    data.depthWrite = this.depthWrite;
////    data.colorWrite = this.colorWrite;
////
////    data.stencilWrite = this.stencilWrite;
////    data.stencilWriteMask = this.stencilWriteMask;
////    data.stencilFunc = this.stencilFunc;
////    data.stencilRef = this.stencilRef;
////    data.stencilFuncMask = this.stencilFuncMask;
////    data.stencilFail = this.stencilFail;
////    data.stencilZFail = this.stencilZFail;
////    data.stencilZPass = this.stencilZPass;
////
////    // rotation (SpriteMaterial)
////    if ( this.rotation !== undefined && this.rotation !== 0 ) data.rotation = this.rotation;
////
////    if ( this.polygonOffset === true ) data.polygonOffset = true;
////    if ( this.polygonOffsetFactor !== 0 ) data.polygonOffsetFactor = this.polygonOffsetFactor;
////    if ( this.polygonOffsetUnits !== 0 ) data.polygonOffsetUnits = this.polygonOffsetUnits;
////
////    if ( this.linewidth !== undefined && this.linewidth !== 1 ) data.linewidth = this.linewidth;
////    if ( this.dashSize !== undefined ) data.dashSize = this.dashSize;
////    if ( this.gapSize !== undefined ) data.gapSize = this.gapSize;
////    if ( this.scale !== undefined ) data.scale = this.scale;
////
////    if ( this.dithering === true ) data.dithering = true;
////
////    if ( this.alphaTest > 0 ) data.alphaTest = this.alphaTest;
////    if ( this.alphaToCoverage === true ) data.alphaToCoverage = this.alphaToCoverage;
////    if ( this.premultipliedAlpha === true ) data.premultipliedAlpha = this.premultipliedAlpha;
////
////    if ( this.wireframe === true ) data.wireframe = this.wireframe;
////    if ( this.wireframeLinewidth > 1 ) data.wireframeLinewidth = this.wireframeLinewidth;
////    if ( this.wireframeLinecap !== 'round' ) data.wireframeLinecap = this.wireframeLinecap;
////    if ( this.wireframeLinejoin !== 'round' ) data.wireframeLinejoin = this.wireframeLinejoin;
////
////    if ( this.flatShading === true ) data.flatShading = this.flatShading;
////
////    if ( this.visible === false ) data.visible = false;
////
////    if ( this.toneMapped === false ) data.toneMapped = false;
////
////    if ( JSON.stringify( this.userData ) !== '{}' ) data.userData = this.userData;
////
////    // TODO: Copied from Object3D.toJSON
////
////    function extractFromCache( cache ) {
////
////        const values = [];
////
////        for ( const key in cache ) {
////
////            const data = cache[ key ];
////            delete data.metadata;
////            values.push( data );
////
////        }
////
////        return values;
////
////    }
////
////    if ( isRootObject ) {
////
////        const textures = extractFromCache( meta.textures );
////        const images = extractFromCache( meta.images );
////
////        if ( textures.length > 0 ) data.textures = textures;
////        if ( images.length > 0 ) data.images = images;
////
////    }
////
////    return data;
////
////}

#endif //THREE_CPP_SRC_MATERIAL_H
