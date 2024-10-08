#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "gl_headers.h"

#include<string>


const std::string REVISION = "139dev";
enum MOUSE { LEFT = 0, MIDDLE = 1, RIGHT = 2, ROTATE = 0, DOLLY = 1, PAN = 2 };
enum TOUCH { TOUCH_ROTATE = 0, TOUCH_PAN = 1, DOLLY_PAN = 2, DOLLY_ROTATE = 3 };

//const int CullFaceNone = 0;
//const int CullFaceBack = 1;
//const int CullFaceFront = 2;
//const int CullFaceFrontBack = 3;
enum class CullFace {
    CullFaceNone = GL_NONE,
    CullFaceBack = GL_BACK,
    CullFaceFront = GL_FRONT,
    CullFaceFrontBack = GL_FRONT_AND_BACK
};

//const int BasicShadowMap = 0;
//const int PCFShadowMap = 1;
//const int PCFSoftShadowMap = 2;
//const int VSMShadowMap = 3;
enum class ShadowMapType {
    BasicShadowMap = 0,
    PCFShadowMap = 1,
    PCFSoftShadowMap =2,
    VSMShadowMap =3,
    NoneShadowMap=4
};

//const int FrontSide = 0;
//const int BackSide = 1;
//const int DoubleSide = 2;
enum class Side {
    None = 0,
    FrontSide	= GL_FRONT,
    BackSide	= GL_BACK,
    DoubleSide	= GL_FRONT_AND_BACK
};
const int FlatShading = 1;
const int SmoothShading = 2;
//const int NoBlending = 0;
//const int NormalBlending = 1;
//const int AdditiveBlending = 2;
//const int SubtractiveBlending = 3;
//const int MultiplyBlending = 4;
//const int CustomBlending = 5;
enum class Blending {
    NoBlending = 0,
    NormalBlending = 1,
    AdditiveBlending = 2,
    SubtractiveBlending = 3,
    MultiplyBlending = 4,
    CustomBlending = 5
};
//const int AddEquation = 100;
//const int SubtractEquation = 101;
//const int ReverseSubtractEquation = 102;
//const int MinEquation = 103;
//const int MaxEquation = 104;
enum class BlendingEquation {
    None					= 0,
    AddEquation				= GL_FUNC_ADD,
    SubtractEquation		= GL_FUNC_SUBTRACT,
    ReverseSubtractEquation = GL_FUNC_REVERSE_SUBTRACT,
    MinEquation				= GL_MIN,
    MaxEquation				= GL_MAX
};

//const int ZeroFactor = 200;
//const int OneFactor = 201;
//const int SrcColorFactor = 202;
//const int OneMinusSrcColorFactor = 203;
//const int SrcAlphaFactor = 204;
//const int OneMinusSrcAlphaFactor = 205;
//const int DstAlphaFactor = 206;
//const int OneMinusDstAlphaFactor = 207;
//const int DstColorFactor = 208;
//const int OneMinusDstColorFactor = 209;
enum class BlendingDstFactor {
    None					= -1,
    ZeroFactor				= GL_ZERO,
    OneFactor				= GL_ONE,
    SrcColorFactor			= GL_SRC_COLOR,
    OneMinusSrcColorFactor	= GL_ONE_MINUS_SRC_COLOR,
    SrcAlphaFactor			= GL_SRC_ALPHA,
    OneMinusSrcAlphaFactor	= GL_ONE_MINUS_SRC_ALPHA,
    DstAlphaFactor			= GL_DST_ALPHA,
    OneMinusDstAlphaFactor	= GL_ONE_MINUS_DST_ALPHA,
    DstColorFactor			= GL_DST_COLOR,
    OneMinusDstColorFactor	= GL_ONE_MINUS_DST_COLOR
};

//const int SrcAlphaSaturateFactor = 210;
enum class BlendingSrcFactor {
    SrcAlphaSaturateFactor = GL_SRC_ALPHA_SATURATE
};

//const int NeverDepth = 0;
//const int AlwaysDepth = 1;
//const int LessDepth = 2;
//const int LessEqualDepth = 3;
//const int EqualDepth = 4;
//const int GreaterEqualDepth = 5;
//const int GreaterDepth = 6;
//const int NotEqualDepth = 7;
enum class DepthModes {
    NeverDepth			= GL_NEVER,
    AlwaysDepth			= GL_ALWAYS,
    LessDepth			= GL_LESS,
    LessEqualDepth		= GL_LEQUAL,
    EqualDepth			= GL_EQUAL,
    GreaterEqualDepth	= GL_GEQUAL,
    GreaterDepth		= GL_GREATER,
    NotEqualDepth		= GL_NOTEQUAL
};
//const int MultiplyOperation = 0;
//const int MixOperation = 1;
//const int AddOperation = 2;
enum class Combine {
    None = 0,
    MultiplyOperation = 1,
    MixOperation = 2,
    AddOperation = 3
};
//const int NoToneMapping = 0;
//const int LinearToneMapping = 1;
//const int ReinhardToneMapping = 2;
//const int CineonToneMapping = 3;
//const int ACESFilmicToneMapping = 4;
//const int CustomToneMapping = 5;
enum class ToneMapping {
    NoToneMapping			= 0,
    LinearToneMapping		= 1,
    ReinhardToneMapping		= 2,
    Uncharted2ToneMapping	= 3,
    CineonToneMapping		= 4,
    ACESFilmicToneMapping	= 5
};

//const int UVMapping = 300;
//const int CubeReflectionMapping = 301;
//const int CubeRefractionMapping = 302;
//const int EquirectangularReflectionMapping = 303;
//const int EquirectangularRefractionMapping = 304;
//const int CubeUVReflectionMapping = 306;

enum class TextureMapping {
    Unknown								= 0,
    UVMapping							= 300,
    CubeReflectionMapping				= 301,
    CubeRefractionMapping				= 302,
    EquirectangularReflectionMapping	= 303,
    EquirectangularRefractionMapping	= 304,
    SphericalReflectionMapping			= 305,
    CubeUVReflectionMapping				= 306,
    CubeUVRefractionMapping				= 307
};

//const int RepeatWrapping = 1000;
//const int ClampToEdgeWrapping = 1001;
//const int MirroredRepeatWrapping = 1002;
enum class Wrapping {
    RepeatWrapping = GL_REPEAT,
    ClampToEdgeWrapping = GL_CLAMP_TO_EDGE,
    MirroredRepeatWrapping = GL_MIRRORED_REPEAT,
    None = GL_NONE
};

//const int NearestFilter = 1003;
//const int NearestMipMapNearestFilter = 1004;
//const int NearestMipMapLinearFilter = 1005;
//const int LinearFilter = 1006;
//const int LinearMipMapNearestFilter = 1007;
//const int LinearMipMapLinearFilter = 1008;
enum class TextureFilter {
    NearestFilter				= GL_NEAREST,
    NearestMipmapNearestFilter	= GL_NEAREST_MIPMAP_NEAREST,
    NearestMipmapLinearFilter	= GL_NEAREST_MIPMAP_LINEAR,
    LinearFilter				= GL_LINEAR,
    LinearMipmapNearestFilter	= GL_LINEAR_MIPMAP_NEAREST,
    LinearMipmapLinearFilter	= GL_LINEAR_MIPMAP_LINEAR
};

//const int UnsignedByteType = 1009;
//const int ByteType = 1010;
//const int ShortType = 1011;
//const int UnsignedShortType = 1012;
//const int IntType = 1013;
//const int UnsignedIntType = 1014;
//const int FloatType = 1015;
//const int HalfFloatType = 1016;
//const int UnsignedShort4444Type = 1017;
//const int UnsignedShort5551Type = 1018;
//const int UnsignedInt248Type = 1020;
enum class TextureDataType {
    None                    = -1,
    UnsignedByteType		= GL_UNSIGNED_BYTE,
    ByteType				= GL_BYTE,
    ShortType				= GL_SHORT,
    UnsignedShortType		= GL_UNSIGNED_SHORT,
    IntType					= GL_INT,
    UnsignedIntType			= GL_UNSIGNED_INT,
    FloatType				= GL_FLOAT,
    HalfFloatType			= GL_HALF_FLOAT,
    UnsignedShort4444Type	= GL_UNSIGNED_SHORT_4_4_4_4,
    UnsignedShort5551Type	= GL_UNSIGNED_SHORT_5_5_5_1,
    UnsignedShort565Type	= GL_UNSIGNED_SHORT_5_6_5,
    UnsignedInt248Type		= GL_UNSIGNED_INT_24_8,
};

enum class TextureTarget {
    None = -1,
    Texture2D = GL_TEXTURE_2D, //A two-dimensional texture.
    cubeMap = GL_TEXTURE_CUBE_MAP, //Positive X face for a cube-mapped texture.
    Texture2DArray = GL_TEXTURE_2D_ARRAY,
    Texture3D = GL_TEXTURE_3D,
    cubeMapPositiveX = GL_TEXTURE_CUBE_MAP_POSITIVE_X, // Positive Y face for a cube-mapped texture.
    cubeMapNegativeX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X, //Negative X face for a cube-mapped texture.
    cubeMapPositiveY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y, // Positive Y face for a cube-mapped texture.
    cubeMapNegativeY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, //Negative Y face for a cube-mapped texture.
    cubeMapPositiveZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z, //Positive Z face for a cube-mapped texture.
    cubeMapNegativeZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z  //Negative Z face for a cube-mapped texture.
};

//const int AlphaFormat = 1021;
//const int RGBFormat = 1022;
//const int RGBAFormat = 1023;
//const int LuminanceFormat = 1024;
//const int LuminanceAlphaFormat = 1025;
//const int DepthFormat = 1026;
//const int DepthStencilFormat = 1027;
//const int RedFormat = 1028;
//const int RedIntegerFormat = 1029;
//const int RGFormat = 1030;
//const int RGIntegerFormat = 1031;
//const int RGBAIntegerFormat = 1033;
enum class PixelFormat {
    AlphaFormat				= GL_ALPHA,
    RGBFormat				= GL_RGB,
    RGBAFormat				= GL_RGBA,
    LuminanceFormat			= GL_LUMINANCE,
    LuminanceAlphaFormat	= GL_LUMINANCE_ALPHA,//GL_LUMINANCE4_ALPHA4,
    RGBEFormat				= GL_RGBA,
    DepthFormat				= GL_DEPTH_COMPONENT,
    DepthStencilFormat		= GL_DEPTH_STENCIL,
    RedFormat				= GL_RED,//GL_R,
    RedIntegerFormat		= GL_R16I,//GL_R16,
    RGFormat				= GL_RG,
    RGIntegerFormat			= GL_RG16I,//GL_RG16,
    RGBIntegerFormat		= GL_RGB16I,
    RGBAIntegerFormat		= GL_RGBA16I,//GL_RGBA16,
    None					= GL_NONE
};

enum class PixelFormatGPU {
    NONE				= GL_NONE,
    ALPHA				= GL_ALPHA,
    RGB					= GL_RGB,
    RGBA				= GL_RGBA,
    LUMINANCE			= GL_LUMINANCE,
    LUMINANCE_ALPHA		= GL_LUMINANCE_ALPHA,
    RED_INTEGER			= GL_RED_INTEGER,
    R8					= GL_R8,
    R8_SNORM			= GL_R8_SNORM,
    R8I					= GL_R8I,
    R8UI				= GL_R8UI,
    R16I				= GL_R16I,
    R16UI				= GL_R16UI,
    R16F				= GL_R16F,
    R32I				= GL_R32I,
    R32UI				= GL_R32UI,
    R32F				= GL_R32F,
    RG8					= GL_RG8,
    RG8_SNORM			= GL_RG8_SNORM,
    RG8I				= GL_RG8I,
    RG8UI				= GL_RG8UI,
    RG16I				= GL_RG16I,
    RG16UI				= GL_RG16UI,
    RG16F				= GL_RG16F,
    RG32I				= GL_RG32I,
    RG32UI				= GL_RG32UI,
    RG32F				= GL_RG32F,
    RGB565				= GL_RGB565,
    RGB8				= GL_RGB8,
    RGB8_SNORM			= GL_RGB8_SNORM,
    RGB8I				= GL_RGB8I,
    RGB8UI				= GL_RGB8UI,
    RGB16I				= GL_RGB16I,
    RGB16UI				= GL_RGB16UI,
    RGB16F				= GL_RGB16F,
    RGB32I				= GL_RGB32I,
    RGB32UI				= GL_RGB32UI,
    RGB32F				= GL_RGB32F,
    RGB9_E5				= GL_RGB9_E5,
    SRGB8				= GL_SRGB8,
    R11F_G11F_B10F		= GL_R11F_G11F_B10F,
    RGBA4				= GL_RGBA4,
    RGBA8				= GL_RGBA8,
    RGBA8_SNORM			= GL_RGBA8_SNORM,
    RGBA8I				= GL_RGBA8I,
    RGBA8UI				= GL_RGBA8UI,
    RGBA16I				= GL_RGBA16I,
    RGBA16UI			= GL_RGBA16UI,
    RGBA16F				= GL_RGBA16F,
    RGBA32I				= GL_RGBA32I,
    RGBA32UI			= GL_RGBA32UI,
    RGBA32F				= GL_RGBA32F,
    RGB5_A1				= GL_RGB5_A1,
    RGB10_A2			= GL_RGB10_A2,
    RGB10_A2UI			= GL_RGB10_A2UI,
    SRGB8_ALPHA8		= GL_SRGB8_ALPHA8,
    DEPTH_COMPONENT16	= GL_DEPTH_COMPONENT16,
    DEPTH_COMPONENT24	= GL_DEPTH_COMPONENT24,
    DEPTH_COMPONENT32F	= GL_DEPTH_COMPONENT32F,
    DEPTH24_STENCIL8	= GL_DEPTH24_STENCIL8,
    DEPTH32F_STENCIL8	= GL_DEPTH32F_STENCIL8
};

const int RGB_S3TC_DXT1_Format = 33776;
const int RGBA_S3TC_DXT1_Format = 33777;
const int RGBA_S3TC_DXT3_Format = 33778;
const int RGBA_S3TC_DXT5_Format = 33779;
const int RGB_PVRTC_4BPPV1_Format = 35840;
const int RGB_PVRTC_2BPPV1_Format = 35841;
const int RGBA_PVRTC_4BPPV1_Format = 35842;
const int RGBA_PVRTC_2BPPV1_Format = 35843;
const int RGB_ETC1_Format = 36196;
const int RGB_ETC2_Format = 37492;
const int RGBA_ETC2_EAC_Format = 37496;
const int RGBA_ASTC_4x4_Format = 37808;
const int RGBA_ASTC_5x4_Format = 37809;
const int RGBA_ASTC_5x5_Format = 37810;
const int RGBA_ASTC_6x5_Format = 37811;
const int RGBA_ASTC_6x6_Format = 37812;
const int RGBA_ASTC_8x5_Format = 37813;
const int RGBA_ASTC_8x6_Format = 37814;
const int RGBA_ASTC_8x8_Format = 37815;
const int RGBA_ASTC_10x5_Format = 37816;
const int RGBA_ASTC_10x6_Format = 37817;
const int RGBA_ASTC_10x8_Format = 37818;
const int RGBA_ASTC_10x10_Format = 37819;
const int RGBA_ASTC_12x10_Format = 37820;
const int RGBA_ASTC_12x12_Format = 37821;
const int RGBA_BPTC_Format = 36492;


//const int LoopOnce = 2200;
//const int LoopRepeat = 2201;
//const int LoopPingPong = 2202;
enum class LoopMode : int {
    Unknown			= 0,
    LoopOnce = 2200,
    LoopRepeat = 2201,
    LoopPingPong = 2202
};

//const int InterpolateDiscrete = 2300;
//const int InterpolateLinear = 2301;
//const int InterpolateSmooth = 2302;
enum class Interpolate : int {
    Unknown			= 0,
    InterpolateDiscrete = 2300,
    InterpolateLinear = 2301,
    InterpolateSmooth = 2302
};

//const int ZeroCurvatureEnding = 2400;
//const int ZeroSlopeEnding = 2401;
//const int WrapAroundEnding = 2402;
enum class InterpolateParam : int {
    Unknown			= 0,
    ZeroCurvatureEnding = 2400,
    ZeroSlopeEnding = 2401,
    WrapAroundEnding = 2402
};

//const int NormalAnimationBlendMode = 2500;
//const int AdditiveAnimationBlendMode = 2501;
enum class AnimationBlendMode : int {
    Unknown			= 0,
    NormalAnimationBlendMode = 2500,
    AdditiveAnimationBlendMode = 2501,
};

const int TrianglesDrawMode = 0;
const int TriangleStripDrawMode = 1;
const int TriangleFanDrawMode = 2;

//const int LinearEncoding = 3000;
//const int sRGBEncoding = 3001;
enum class TextureEncoding : int {
    Unknown			= 0,
    LinearEncoding	= 3000,
    sRGBEncoding	= 3001,
    GammaEncoding	= 3007,
    RGBEEncoding	= 3002,
    LogLuvEncoding	= 3003,
    RGBM7Encoding	= 3004,
    RGBM16Encoding	= 3005,
    RGBDEncoding	= 3006
};


//const int BasicDepthPacking = 3200;
//const int RGBADepthPacking = 3201;
enum class DepthPackingStrategies {
    None=0,
    BasicDepthPacking = 3200,
    RGBADepthPacking = 3201
};
//const int TangentSpaceNormalMap = 0;
//const int ObjectSpaceNormalMap = 1;
enum class NormalMapTypes {
    TangentSpaceNormalMap	= 0,
    ObjectSpaceNormalMap	= 1,
    None = 2
};


constexpr char NoColorSpace[] = "";
constexpr char SRGBColorSpace[] = "srgb";
constexpr char LinearSRGBColorSpace[] = "srgb-linear";


//const int ZeroStencilOp = 0;
//const int KeepStencilOp = 7680;
//const int ReplaceStencilOp = 7681;
//const int IncrementStencilOp = 7682;
//const int DecrementStencilOp = 7683;
//const int IncrementWrapStencilOp = 34055;
//const int DecrementWrapStencilOp = 34056;
//const int InvertStencilOp = 5386;
enum class StencilOp {
    ZeroStencilOp			= 0,
    KeepStencilOp			= 7680,
    ReplaceStencilOp		= 7681,
    IncrementStencilOp		= 7682,
    DecrementStencilOp		= 7683,
    IncrementWrapStencilOp	= 34055,
    DecrementWrapStencilOp	= 34056,
    InvertStencilOp			= 5386,
};

//const int NeverStencilFunc = 512;
//const int LessStencilFunc = 513;
//const int EqualStencilFunc = 514;
//const int LessEqualStencilFunc = 515;
//const int GreaterStencilFunc = 516;
//const int NotEqualStencilFunc = 517;
//const int GreaterEqualStencilFunc = 518;
//const int AlwaysStencilFunc = 519;
enum class
        StencilFunc {
    NeverStencilFunc		= GL_NEVER,
    LessStencilFunc			= GL_LESS,
    EqualStencilFunc		= GL_EQUAL,
    LessEqualStencilFunc	= GL_LEQUAL,
    NotEqualStencilFunc		= GL_NOTEQUAL,
    GreaterEqualStencilFunc = GL_GEQUAL,
    AlwaysStencilFunc		= GL_ALWAYS
};

//const int StaticDrawUsage = 35044;
//const int DynamicDrawUsage = 35048;
//const int StreamDrawUsage = 35040;
//const int StaticReadUsage = 35045;
//const int DynamicReadUsage = 35049;
//const int StreamReadUsage = 35041;
//const int StaticCopyUsage = 35046;
//const int DynamicCopyUsage = 35050;
//const int StreamCopyUsage = 35042;
enum class Usage {
    StaticDrawUsage		= 35044,
    DynamicDrawUsage	= 35048,
    StreamDrawUsage		= 35040,
    StaticReadUsage		= 35045,
    DynamicReadUsage	= 35049,
    StreamReadUsage		= 35041,
    StaticCopyUsage		= 35046,
    DynamicCopyUsage	= 35050,
    StreamCopyUsage		= 35042
};

enum class BufferUsageHint {
    Float			= GL_FLOAT,
    UnsignedShort	= GL_UNSIGNED_SHORT,
    Short			= GL_SHORT,
    UnsignedInt		= GL_UNSIGNED_INT,
    Int				= GL_INT,
    Byte			= GL_BYTE,
    UnsignedByte	= GL_UNSIGNED_BYTE
};

enum class UniformType {
    None		= 0,
    Float		= GL_FLOAT,
    FloatVect2	= GL_FLOAT_VEC2,
    FloatVec3	= GL_FLOAT_VEC3,
    FloatVec4	= GL_FLOAT_VEC4,
    Int			= GL_INT,
    IntVec2		= GL_INT_VEC2,
    IntVec3		= GL_INT_VEC3,
    IntVect4	= GL_INT_VEC4,
    Bool		= GL_BOOL,
    BoolVec2	= GL_BOOL_VEC2,
    BoolVec3	= GL_BOOL_VEC3,
    BoolVec4	= GL_BOOL_VEC4,
    FloatMat2	= GL_FLOAT_MAT2,
    FloatMat3	= GL_FLOAT_MAT3,
    FloatMat4	= GL_FLOAT_MAT4,
    Sampler2D	= GL_SAMPLER_2D,
    Sampler3D   = GL_SAMPLER_3D,
    SamplerCube = GL_SAMPLER_CUBE,
    Sampler2DArray = GL_SAMPLER_2D_ARRAY
};

enum class UniformClassType : unsigned {
    SingleUniform,
    PureArrayUniform,
    StructuredUniform
};

enum class Extension : size_t{
    ARB_depth_texture = 1,
    EXT_frag_depth = 1 << 1,
    EXT_texture_filter_anisotropic = 1 << 2,
    EXT_blend_minmax = 1 << 3,
    EXT_texture_compression_s3tc = 1 << 4,
    EXT_shader_texture_lod = 1 << 5,
    OES_texture_float = 1 << 6,
    OES_texture_float_linear = 1 << 7,
    OES_texture_half_float = 1 << 8,
    OES_texture_half_float_linear = 1 << 9,
    OES_standard_derivatives = 1 << 10,
    ANGLE_instanced_arrays = 1 << 11,
    OES_element_index_uint = 1 << 12,
    EXT_draw_buffers = 1 << 13,
    EXT_draw_buffers2 = 1 << 14,
    ARB_instanced_arrays = 1<<15,
    ARB_ES3_compatibility=1<<16
};

enum class ShaderLibID : unsigned {
    alphamap_fragment = 0,
    alphamap_pars_fragment,
    alphatest_fragment,
    alphatest_pars_fragment,
    aomap_fragment,
    aomap_pars_fragment,
    begin_vertex,
    beginnormal_vertex,
    bsdfs,
    bumpmap_pars_fragment,
    clearcoat_normal_fragment_begin,
    clearcoat_normal_fragment_maps,
    clearcoat_pars_fragment,
    clipping_planes_fragment,
    clipping_planes_pars_fragment,
    clipping_planes_pars_vertex,
    clipping_planes_vertex,
    color_fragment,
    color_pars_fragment,
    color_pars_vertex,
    color_vertex,
    common,
    cube_uv_reflection_fragment,
    default_vertex,
    default_fragment,
    defaultnormal_vertex,
    displacementmap_pars_vertex,
    displacementmap_vertex,
    dithering_fragment,
    dithering_pars_fragment,
    emissivemap_fragment,
    emissivemap_pars_fragment,
    encodings_fragment,
    encodings_pars_fragment,
    envmap_fragment,
    envmap_common_pars_fragment,
    envmap_pars_fragment,
    envmap_pars_vertex,
    envmap_physical_pars_fragment,
    envmap_vertex,
    fog_vertex,
    fog_pars_vertex,
    fog_fragment,
    fog_pars_fragment,
    gradientmap_pars_fragment,
    iridescence_fragment,
    iridescence_pars_fragment,
    lightmap_fragment,
    lightmap_pars_fragment,
    lights_fragment_begin,
    lights_fragment_end,
    lights_fragment_maps,
    //check from this point
    lights_lambert_vertex,
    lights_lambert_fragment,
    lights_lambert_pars_fragment,
    lights_pars_begin,
    lights_toon_fragment,
    lights_toon_pars_fragment,
    lights_phong_fragment,
    lights_phong_pars_fragment,
    lights_physical_fragment,
    lights_physical_pars_fragment,
    logdepthbuf_fragment,
    logdepthbuf_pars_fragment,
    logdepthbuf_pars_vertex,
    logdepthbuf_vertex,
    map_fragment,
    map_pars_fragment,
    map_particle_fragment,
    map_particle_pars_fragment,
    metalnessmap_fragment,
    metalnessmap_pars_fragment,
    morphcolor_vertex,
    morphnormal_vertex,
    morphtarget_pars_vertex,
    morphtarget_vertex,
    normal_fragment_begin,
    normal_fragment_maps,
    normal_vertex,
    normal_pars_fragment,
    normal_pars_vertex,
    normalmap_pars_fragment,
    packing,
    premultiplied_alpha_fragment,
    project_vertex,
    roughnessmap_fragment,
    roughnessmap_pars_fragment,
    shadowmap_pars_fragment,
    shadowmap_pars_vertex,
    shadowmap_vertex,
    shadowmask_pars_fragment,
    skinbase_vertex,
    skinning_pars_vertex,
    skinning_vertex,
    skinnormal_vertex,
    specularmap_fragment,
    specularmap_pars_fragment,
    tonemapping_fragment,
    tonemapping_pars_fragment,
    uv_pars_fragment,
    uv_pars_vertex,
    uv_vertex,
    uv2_pars_fragment,
    uv2_pars_vertex,
    uv2_vertex,
    worldpos_vertex,
    background_frag,
    background_vert,
    backgroundCube_frag,
    backgroundCube_vert,
    cube_frag,
    cube_vert,
    depth_frag,
    depth_vert,
    distanceRGBA_frag,
    distanceRGBA_vert,
    equirect_frag,
    equirect_vert,
    linedashed_frag,
    linedashed_vert,
    meshbasic_frag,
    meshbasic_vert,
    meshlambert_frag,
    meshlambert_vert,
    meshmatcap_frag,
    meshmatcap_vert,
    meshtoon_frag,
    meshtoon_vert,
    meshphong_frag,
    meshphong_vert,
    meshphysical_frag,
    meshphysical_vert,
    meshnormal_frag,
    meshnormal_vert,
    points_frag,
    points_vert,
    shadow_frag,
    shadow_vert,
    sprite_frag,
    sprite_vert,
    vsm_vert,
    vsm_frag,
    transmissionmap_fragment,
    transmissionmap_pars_fragment,
    transmission_fragment,
    transmission_pars_fragment,

    output_fragment,
    unknown=999
};

constexpr char GLSL1[] = "100";
constexpr char GLSL3[] = "300 es";

enum class GLSLVersion {
    GLSL1,
    GLSL3
};

const int _SRGBAFormat = 1035; // fallback for WebGL 1

#endif /* CONSTANTS_H */
