#ifndef CONSTANTS_H
#define CONSTANTS_H

#include<string>
#include "gl_headers.h"

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
const int BasicShadowMap = 0;
const int PCFShadowMap = 1;
const int PCFSoftShadowMap = 2;
const int VSMShadowMap = 3;
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
//const int SrcAlphaSaturateFactor = 210;
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
const int MultiplyOperation = 0;
const int MixOperation = 1;
const int AddOperation = 2;
const int NoToneMapping = 0;
const int LinearToneMapping = 1;
const int ReinhardToneMapping = 2;
const int CineonToneMapping = 3;
const int ACESFilmicToneMapping = 4;
const int CustomToneMapping = 5;

const int UVMapping = 300;
const int CubeReflectionMapping = 301;
const int CubeRefractionMapping = 302;
const int EquirectangularReflectionMapping = 303;
const int EquirectangularRefractionMapping = 304;
const int CubeUVReflectionMapping = 306;
const int RepeatWrapping = 1000;
const int ClampToEdgeWrapping = 1001;
const int MirroredRepeatWrapping = 1002;
const int NearestFilter = 1003;
const int NearestMipmapNearestFilter = 1004;
const int NearestMipMapNearestFilter = 1004;
const int NearestMipmapLinearFilter = 1005;
const int NearestMipMapLinearFilter = 1005;
const int LinearFilter = 1006;
const int LinearMipmapNearestFilter = 1007;
const int LinearMipMapNearestFilter = 1007;
const int LinearMipmapLinearFilter = 1008;
const int LinearMipMapLinearFilter = 1008;
const int UnsignedByteType = 1009;
const int ByteType = 1010;
const int ShortType = 1011;
const int UnsignedShortType = 1012;
const int IntType = 1013;
const int UnsignedIntType = 1014;
const int FloatType = 1015;
const int HalfFloatType = 1016;
const int UnsignedShort4444Type = 1017;
const int UnsignedShort5551Type = 1018;
const int UnsignedInt248Type = 1020;
const int AlphaFormat = 1021;
const int RGBFormat = 1022;
const int RGBAFormat = 1023;
const int LuminanceFormat = 1024;
const int LuminanceAlphaFormat = 1025;
const int DepthFormat = 1026;
const int DepthStencilFormat = 1027;
const int RedFormat = 1028;
const int RedIntegerFormat = 1029;
const int RGFormat = 1030;
const int RGIntegerFormat = 1031;
const int RGBAIntegerFormat = 1033;

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
const int LoopOnce = 2200;
const int LoopRepeat = 2201;
const int LoopPingPong = 2202;
const int InterpolateDiscrete = 2300;
const int InterpolateLinear = 2301;
const int InterpolateSmooth = 2302;
const int ZeroCurvatureEnding = 2400;
const int ZeroSlopeEnding = 2401;
const int WrapAroundEnding = 2402;
const int NormalAnimationBlendMode = 2500;
const int AdditiveAnimationBlendMode = 2501;
const int TrianglesDrawMode = 0;
const int TriangleStripDrawMode = 1;
const int TriangleFanDrawMode = 2;
const int LinearEncoding = 3000;
const int sRGBEncoding = 3001;
const int BasicDepthPacking = 3200;
const int RGBADepthPacking = 3201;
const int TangentSpaceNormalMap = 0;
const int ObjectSpaceNormalMap = 1;

// Color space string identifiers, matching CSS Color Module Level 4 and WebGPU names where available.
const std::string NoColorSpace = "";
const std::string SRGBColorSpace = "srgb";
const std::string LinearSRGBColorSpace = "srgb-linear";

const int ZeroStencilOp = 0;
const int KeepStencilOp = 7680;
const int ReplaceStencilOp = 7681;
const int IncrementStencilOp = 7682;
const int DecrementStencilOp = 7683;
const int IncrementWrapStencilOp = 34055;
const int DecrementWrapStencilOp = 34056;
const int InvertStencilOp = 5386;

const int NeverStencilFunc = 512;
const int LessStencilFunc = 513;
const int EqualStencilFunc = 514;
const int LessEqualStencilFunc = 515;
const int GreaterStencilFunc = 516;
const int NotEqualStencilFunc = 517;
const int GreaterEqualStencilFunc = 518;
const int AlwaysStencilFunc = 519;

const int StaticDrawUsage = 35044;
const int DynamicDrawUsage = 35048;
const int StreamDrawUsage = 35040;
const int StaticReadUsage = 35045;
const int DynamicReadUsage = 35049;
const int StreamReadUsage = 35041;
const int StaticCopyUsage = 35046;
const int DynamicCopyUsage = 35050;
const int StreamCopyUsage = 35042;

const std::string GLSL1 = "100";
const std::string GLSL3 = "300 es";

const int _SRGBAFormat = 1035; // fallback for WebGL 1

#endif /* CONSTANTS_H */
