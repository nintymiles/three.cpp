#ifndef SRC_COMMON_TYPES_H_
#define SRC_COMMON_TYPES_H_

#include <GLES3/gl3.h>

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








}

#endif /* SRC_COMMON_TYPES_H_ */
