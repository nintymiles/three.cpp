#ifndef SRC_COMMON_TYPES_H_
#define SRC_COMMON_TYPES_H_

#include <GLES3/gl3.h>

#include <unordered_map>

namespace threecpp {

struct Range{
    int start;
    int count;

    //Range(const Range& r):start(r.start),count(r.count){};
};


struct GeometryGroup{
    int start;
    int count;
    int materialIndex;
};

union GeometryValue{
    double d;
    int i;
};

union UniformValue{
    double d;
    int i;
    unsigned u;
};

struct GLActiveInfo{
    char* name;
    GLint size;
    GLenum type;
};

struct GLVertexAttribute{
    GLenum type;
    GLint location;
    GLint locationSize;
};

struct GLViewPort
{
    float x;
    float y;
    float width;
    float height;

    GLViewPort(float x,float y,float width,float height):x(x),y(y),width(width),height(height){};
};

enum class LightType{
    Light,DirectionalLight,SpotLight,PointLight,AmbientLight,LightProbe,HemisphereLight,RectAreaLight
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

enum class Side {
    None = 0,
    FrontSide	= GL_FRONT,
    BackSide	= GL_BACK,
    DoubleSide	= GL_FRONT_AND_BACK
};

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

enum class BlendingEquation {
    None					= 0,
    AddEquation				= GL_FUNC_ADD,
    SubtractEquation		= GL_FUNC_SUBTRACT,
    ReverseSubtractEquation = GL_FUNC_REVERSE_SUBTRACT,
    MinEquation				= GL_MIN,
    MaxEquation				= GL_MAX
};

enum class Blending {
    NoBlending = 0,
    NormalBlending = 1,
    AdditiveBlending = 2,
    SubtractiveBlending = 3,
    MultiplyBlending = 4,
    CustomBlending = 5
};

enum class BlendingSrcFactor {
    SrcAlphaSaturateFactor = GL_SRC_ALPHA_SATURATE
};

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

enum class StencilFunc {
    NeverStencilFunc		= GL_NEVER,
    LessStencilFunc			= GL_LESS,
    EqualStencilFunc		= GL_EQUAL,
    LessEqualStencilFunc	= GL_LEQUAL,
    NotEqualStencilFunc		= GL_NOTEQUAL,
    GreaterEqualStencilFunc = GL_GEQUAL,
    AlwaysStencilFunc		= GL_ALWAYS
};

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

enum class ToneMapping {
    NoToneMapping			= 0,
    LinearToneMapping		= 1,
    ReinhardToneMapping		= 2,
    Uncharted2ToneMapping	= 3,
    CineonToneMapping		= 4,
    ACESFilmicToneMapping	= 5
};

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

enum class TextureFilter {
    NearestFilter				= GL_NEAREST,
    NearestMipmapNearestFilter	= GL_NEAREST_MIPMAP_NEAREST,
    NearestMipMapNearestFilter	= GL_NEAREST_MIPMAP_NEAREST,
    NearestMipmapLinearFilter	= GL_NEAREST_MIPMAP_LINEAR,
    NearestMipMapLinearFilter	= GL_NEAREST_MIPMAP_LINEAR,
    LinearFilter				= GL_LINEAR,
    LinearMipmapNearestFilter	= GL_LINEAR_MIPMAP_NEAREST,
    LinearMipMapNearestFilter	= GL_LINEAR_MIPMAP_NEAREST,
    LinearMipmapLinearFilter	= GL_LINEAR_MIPMAP_LINEAR,
    LinearMipMapLinearFilter	= GL_LINEAR_MIPMAP_LINEAR
};

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

enum class TextureDataType {
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

enum class Wrapping {
    RepeatWrapping = GL_REPEAT,
    ClampToEdgeWrapping = GL_CLAMP_TO_EDGE,
    MirroredRepeatWrapping = GL_MIRRORED_REPEAT,
    None = GL_NONE
};


enum class PixelFormat {
    AlphaFormat				= GL_ALPHA,
    RGBFormat				= GL_RGB,
    RGBAFormat				= GL_RGBA,
    LuminanceFormat			= GL_LUMINANCE,
    LuminanceAlphaFormat	= GL_LUMINANCE4_ALPHA4,
    RGBEFormat				= GL_RGBA,
    DepthFormat				= GL_DEPTH,
    DepthStencilFormat		= GL_DEPTH_STENCIL,
    RedFormat				= GL_R,
    RedIntegerFormat		= GL_R16,
    RGFormat				= GL_RG,
    RGIntegerFormat			= GL_RG16,
    RGBIntegerFormat		= GL_RGB16,
    RGBAIntegerFormat		= GL_RGBA16,
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

struct EnumHash {
    template <typename T>
    typename std::underlying_type<T>::type operator()(T t) const {
        return static_cast<typename std::underlying_type<T>::type>(t);
    }
};

template<typename EnumT,typename ValueT>
using enum_map = std::unordered_map<EnumT, ValueT, EnumHash>;






}

#endif /* SRC_COMMON_TYPES_H_ */
