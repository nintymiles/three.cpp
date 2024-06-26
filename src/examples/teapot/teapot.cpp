//
// Created by sf on 8/9/17.
//

#include "teapot.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "teapot.inl"
#include <vector>
//#include <string>
#include "logger.h"




#define GL_PROFILE_GL3

GLuint Teapot::g_vao = 0;
static struct android_app*  g_App = NULL;

static const std::vector<std::pair<const char*, GLuint>> faces {
        {"skybox-negx.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_X},
        {"skybox-negy.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y},
        {"skybox-negz.jpg", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z},
        {"skybox-posx.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_X},
        {"skybox-posy.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_Y},
        {"skybox-posz.jpg", GL_TEXTURE_CUBE_MAP_POSITIVE_Z}
};

const char* vtxShader =
#ifdef GL_PROFILE_GL3
"#version 300 es\n"
#else
"#version 300 es\n"
#endif
"in vec3 g_Position;\n"
"in vec3 g_TexCoord0;\n"
"in vec3 g_Tangent;\n"
"in vec3 g_Binormal;\n"
"in vec3 g_Normal;\n"
"\n"
"uniform mat4 world;\n"
"uniform mat4 worldInverseTranspose;\n"
"uniform mat4 worldViewProj;\n"
"uniform mat4 viewInverse;\n"
"\n"
"out vec2 texCoord;\n"
"out vec3 worldEyeVec;\n"
"out vec3 worldNormal;\n"
"out vec3 worldTangent;\n"
"out vec3 worldBinorm;\n"
"\n"
"void main() {\n"
"  gl_Position = worldViewProj * vec4(g_Position, 1.0);\n"
"  texCoord.x = g_TexCoord0.x;\n"
"  texCoord.y = 1.0-g_TexCoord0.y;\n"
"  worldNormal = (worldInverseTranspose * vec4(g_Normal, 1.0)).xyz;\n"
"  worldTangent = (worldInverseTranspose * vec4(g_Tangent, 1.0)).xyz;\n"
"  worldBinorm = (worldInverseTranspose * vec4(g_Binormal, 1.0)).xyz;\n"
"  vec3 worldPos = (world * vec4(g_Position, 1.0)).xyz;\n"
"  worldEyeVec = normalize(worldPos - viewInverse[3].xyz);\n"
"}";

const char* fragShader =
#ifdef GL_PROFILE_GL3
"#version  300 es\n"
"precision mediump float;\n"
#else
"#version  300 es\n"
"precision mediump float;\n"
#endif
"const float bumpHeight = 0.5;\n"
"uniform sampler2D normalSampler;\n"
"uniform samplerCube envSampler;\n"
"\n"
"in vec2 texCoord;\n"
"in vec3 worldEyeVec;\n"
"in vec3 worldNormal;\n"
"in vec3 worldTangent;\n"
"in vec3 worldBinorm;\n"
"// the outputing fragment color\n"
"layout(location = 0) out vec4 fragColor;\n"
"\n"
"void main() {\n"
"  vec2 bump = (texture(normalSampler, texCoord.xy).xy * 2.0 - 1.0) * bumpHeight;\n"
"  vec3 normal = normalize(worldNormal);\n"
"  vec3 tangent = normalize(worldTangent);\n"
"  vec3 binormal = normalize(worldBinorm);\n"
"  vec3 nb = normal + bump.x * tangent + bump.y * binormal;\n"
"  nb = normalize(nb);\n"
"  vec3 worldEye = normalize(worldEyeVec);\n"
"  vec3 lookup = reflect(worldEye, nb);\n"
"  vec4 color = texture(envSampler, lookup);\n"
"  fragColor = color;\n"
"}";

Teapot::Teapot() : num_vertices(0), num_indices(0), ibo(0), vbo(0), tex_skybox(0),
                   tex_bump(0), program(0), rotX(0.0f), rotY(0.0f), zoom(1.0f),
                   camRX(0.0f), camRY(0.0f),
                   addRotX(0.0f), addRotY(0.0f), addZoom(0.0f), addCamRX(0.0f), addCamRY(0.0f){ }

Teapot::~Teapot()
{
    if (program)
    {
        glDeleteProgram(program);
    }
    if (ibo)
    {
        glDeleteBuffers(1, &ibo);
    }
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
    }
    if (tex_skybox)
    {
        glDeleteTextures(1, &tex_skybox);
    }
    if (tex_bump)
    {
        glDeleteTextures(1, &tex_bump);
    }
}

#define glCheckError() {GLenum error = glGetError(); if (error != GL_NO_ERROR) {Log(LOG_ERROR) << "GL error at " << __FILE__ << "@" << __LINE__ << ": " << error << " (" << errorToStr(error) << ")";}}

static const char* errorToStr(GLenum error)
{
    switch(error)
    {
        case GL_INVALID_ENUM:
            return "GL_INVALID_ENUM";
        case GL_INVALID_OPERATION:
            return "GL_INVALID_OPERATION";
        case GL_INVALID_VALUE:
            return "GL_INVALID_VALUE";
#ifdef GL_INVALID_INDEX
        case GL_INVALID_INDEX:
            return "GL_INVALID_INDEX";
#endif // GL_INVALID_INDEX
        default:
            return "Unknown error";
    }
}

#ifdef __ANDROID__
bool Teapot::init(struct android_app* app)
{
    g_App = app;
#else
bool Teapot::init()
{
#endif

    if (g_vao == 0)
    {
        glGenVertexArrays(1, &g_vao);
        glBindVertexArray(g_vao);
    }

    num_indices = sizeof(teapotIndices) / sizeof(teapotIndices[0]);
    num_vertices = (sizeof(teapotPositions) / sizeof(teapotPositions[0])) / 3;

    std::vector<vtxData> bufferData(num_vertices);
    for (int i = 0; i < num_vertices; ++i)
    {
        bufferData[i].pos = Vector3{teapotPositions[3*i], teapotPositions[3*i+1], teapotPositions[3*i+2]};
        bufferData[i].norm = Vector3{teapotNormals[3*i], teapotNormals[3*i+1], teapotNormals[3*i+2]};
        bufferData[i].tangent = Vector3{teapotTangents[3*i], teapotTangents[3*i+1], teapotTangents[3*i+2]};
        bufferData[i].binorm = Vector3{teapotBinormals[3*i], teapotBinormals[3*i+1], teapotBinormals[3*i+2]};
        bufferData[i].texcoord = Vector3{teapotTexCoords[3*i], teapotTexCoords[3*i+1], teapotTexCoords[3*i+2]};
    }

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(vtxData), bufferData.data(), GL_STATIC_DRAW);

    glCheckError();

    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(teapotIndices), teapotIndices, GL_STATIC_DRAW);

    glCheckError();

    glGenTextures(1, &tex_skybox);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_skybox);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    for(const auto& face: faces)
    {
        int x, y, channels;
        //auto data = stbi_load(face.first, &x, &y, &channels, 0);
#ifdef __ANDROID__
        std::vector<uint8_t> fileData;
        Teapot::GetAssetData(face.first,fileData);
        auto data = stbi_load_from_memory(fileData.data(),fileData.size(), &x, &y, &channels, 0);
#else
        auto data = stbi_load(face.first, &x, &y, &channels, 0);
#endif
        if (data == NULL)
        {
            Log(LOG_ERROR) << "Could not load " << face.first;
        }
        glTexImage2D(face.second, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glCheckError();

    glGenTextures(1, &tex_bump);
    glBindTexture(GL_TEXTURE_2D, tex_bump);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    {
        int x, y, channels;
        //auto data = stbi_load("bump.jpg", &x, &y, &channels, 0);
#ifdef __ANDROID__
        std::vector<uint8_t> bumpData;
        Teapot::GetAssetData("bump.jpg",bumpData);
        auto data = stbi_load_from_memory(bumpData.data(),bumpData.size(), &x, &y, &channels, 0);
#else
        auto data = stbi_load("bump.jpg", &x, &y, &channels, 0);
#endif
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    glCheckError();

    if (!compileShaders())
    {
        Log(LOG_ERROR) << "Initialization failed";
        return false;
    }
    glCheckError();

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLfloat aspect = (GLfloat)viewport[2] / (GLfloat)viewport[3];

    Matrix4 projection = Matrix4().makePerspective(45.0f, aspect, 10.0f, 500.0f);
    Matrix4 view = Matrix4().makeTranslation(0.0f, -10.0f, -100.0f);
    Matrix4 model = Matrix4().makeTranslation(0.0f, -10.0f, 0.0f);
    Vector3 axisX = Vector3(1.0f, 0.0f, 0.0f);
    model.makeRotationAxis(axisX,Number::PI/2);
        //rotate(model, (float)M_PI_2, Vector3(1.0f, 0.0f, 0.0f));

    Matrix4 mvp = Matrix4(projection).multiply(view).multiply(model);
    Matrix4 worldInverseTranspose = model.clone().invert();
    worldInverseTranspose.transpose();
    //worldInverseTranspose = glm::transpose(worldInverseTranspose);
    Matrix4 viewInverse = view.clone().invert();

    unfData.world = model;
    unfData.worldInverseTranspose = worldInverseTranspose;
    unfData.viewInverse = viewInverse;
    unfData.worldViewProj = mvp;

    return true;
}

#ifdef __ANDROID__
// Helper to retrieve data placed into the assets/ directory (android/app/src/main/assets)
int Teapot::GetAssetData(const char* filename, std::vector<uint8_t>& buf)
{
    int num_bytes = 0;
    AAsset* asset_descriptor = AAssetManager_open(g_App->activity->assetManager, filename, AASSET_MODE_BUFFER);
    if (asset_descriptor)
    {
        num_bytes = AAsset_getLength(asset_descriptor);

        buf.resize(num_bytes);
        int64_t num_bytes_read = AAsset_read(asset_descriptor, buf.data(), buf.size());

        AAsset_close(asset_descriptor);
        //IM_ASSERT(num_bytes_read == num_bytes);
    }
    return num_bytes;
}
#endif

float Teapot::zoomValue() {return zoom;}

void Teapot::draw()
{
    // Apply rotations and zoom, recalculate matrices
    rotX += addRotX;
    rotY += addRotY;
    zoom += addZoom;

    if (zoom > 3.0f)
    {
        zoom = 3.0f;
    }
    if (zoom < 0.5f)
    {
        zoom = 0.5f;
    }

    camRX += addCamRX;
    camRY += addCamRY;

    const GLfloat clampEps = 0.00001f;

    if (camRX > M_PI * 2)
    {
        camRX -= M_PI * 2;
    }
    if (camRX < - M_PI * 2)
    {
        camRX += M_PI * 2;
    }
    if (camRY > (M_PI_2 - clampEps))
    {
        camRY = M_PI_2 - clampEps;
    }
    if (camRY < - (M_PI_2 - clampEps))
    {
        camRY = - (M_PI_2 - clampEps);
    }

    addRotX = 0;
    addRotY = 0;
    addZoom = 0;

    addCamRX = 0;
    addCamRY = 0;

    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    GLfloat aspect = (GLfloat) viewport[2] / (GLfloat) viewport[3];

    const GLfloat camDistance = 100.0f;

    Vector3 cameraPos = Vector3(camDistance * cos(camRX) * cos(camRY), camDistance * sin(camRY), camDistance * sin(camRX) * cos(camRY));

    //cameraPos = (glm::rotate(Matrix4(1.0f), rotX, Vector3(0.0f, 1.0f, 0.0f)) * glm::rotate(glm::mat4(1.0f), rotY, Vector3(0.0f, 0.0f, 1.0f)) * glm::vec4(cameraPos, 1.0f));

    Matrix4 projection = Matrix4().makePerspective(MathUtils::DEG2RAD * (65.0f / zoom), aspect, 20.0f, 500.0f);

    Vector3 target{10,-200,-100},up{0.0f, 1.0f, 0.0f};
    Matrix4 view = Matrix4().lookAt(cameraPos, target, up);
    /*Matrix4 model = glm::rotate(glm::translate(glm::mat4(1.0f), Vector3(0.0f, -10.0f, 0.0f)),
                                  -(GLfloat)M_PI_2,
                                  Vector3(1.0f, 0.0f, 0.0f)); */
    Matrix4 model{};
    model.makeTranslation(10.0f, -200.0f, -100.0f);
    model.makeRotationZ(rotY);
    model.makeRotationY(-rotX);
    model.makeRotationX(Number::PI/2);
//    model = glm::rotate(model, rotY, Vector3(0.0f, 0.0f, 1.0f));
//    model = glm::rotate(model, -rotX, Vector3(0.0f, 1.0f, 0.0f));
//    model = glm::rotate(model, -(GLfloat)M_PI_2, Vector3(1.0f, 0.0f, 0.0f));

    Matrix4 mvp = Matrix4(projection).multiply(view).multiply(model);
    Matrix4 worldInverseTranspose = model.clone().invert();
    Matrix4 viewInverse = view.clone().invert();

    unfData.world = model;
    unfData.worldInverseTranspose = worldInverseTranspose;
    unfData.viewInverse = viewInverse;
    unfData.worldViewProj = mvp;

    glUseProgram(program);
#ifdef GL_PROFILE_GL3
    glBindVertexArray(g_vao);
#endif

    glEnable(GL_DEPTH_TEST);

    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckError();
    glVertexAttribPointer(attribs.g_Position, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, pos));
    glEnableVertexAttribArray(attribs.g_Position);
    glCheckError();

    glVertexAttribPointer(attribs.g_Normal, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, norm));
    glEnableVertexAttribArray(attribs.g_Normal);
    glCheckError();

    glVertexAttribPointer(attribs.g_TexCoord0, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, texcoord));
    glEnableVertexAttribArray(attribs.g_TexCoord0);
    glCheckError();

    glVertexAttribPointer(attribs.g_Tangent, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, tangent));
    glEnableVertexAttribArray(attribs.g_Tangent);
    glCheckError();

    glVertexAttribPointer(attribs.g_Binormal, 3, GL_FLOAT, GL_FALSE, sizeof(vtxData), (void*)offsetof(vtxData, binorm));
    glEnableVertexAttribArray(attribs.g_Binormal);
    glCheckError();

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glCheckError();

    glUniformMatrix4fv(uniforms.world, 1, GL_FALSE, (GLfloat*)&unfData.world.elements);
    glUniformMatrix4fv(uniforms.worldViewProj, 1, GL_FALSE, (GLfloat*)&unfData.worldViewProj.elements);
    glUniformMatrix4fv(uniforms.worldInverseTranspose, 1, GL_FALSE, (GLfloat*)&unfData.worldInverseTranspose.elements);
    glUniformMatrix4fv(uniforms.viewInverse, 1, GL_FALSE, (GLfloat*)&unfData.viewInverse.elements);

    glCheckError();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex_bump);
    glUniform1i(uniforms.normalSampler, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex_skybox);
    glUniform1i(uniforms.envSampler, 1);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, 0);
}

void Teapot::rotateBy(float angleX, float angleY)
{
    addRotX += angleX;
    addRotY += angleY;
}

void Teapot::rotateTo(float angleX)
{
    rotX = angleX;
}

void Teapot::rotateCameraTo(float angleX)
{
    camRX = angleX;
}

void Teapot::rotateCameraBy(float angleX, float angleY)
{
    addCamRX -= angleX;
    addCamRY -= angleY;
}

void Teapot::zoomBy(float zoomFactor)
{
    addRotX = 0;
    addRotY = 0;
    addCamRX = 0;
    addCamRY = 0;
    addZoom += zoomFactor;
}

static GLint compileShader(GLenum shaderType, const char* shaderSrc)
{
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrc, NULL);
    glCompileShader(shader);
    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE)
    {
        // Assume we only care about vertex and fragment shaders
        Log(LOG_ERROR) << "Could not compile shader! Shader type: " << ((shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment");
        GLint logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength + 1);
        glGetShaderInfoLog(shader, infoLog.size(), &logLength, infoLog.data());
        Log(LOG_ERROR) << "Error log: " << infoLog.data();
        Log(LOG_ERROR) << "Shader source: " << shaderSrc;
        glDeleteShader(shader);
        return -1;
    }
    return shader;
}

bool Teapot::compileShaders() {
    GLint vertexShader = compileShader(GL_VERTEX_SHADER, vtxShader);
    GLint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragShader);
    if (vertexShader < 0 || fragmentShader < 0)
    {
        // Delete any shaders that were actually compiled
        if (vertexShader >= 0) {glDeleteShader(vertexShader);}
        if (fragmentShader >= 0) {glDeleteShader(fragmentShader);}
        return false;
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE)
    {
        Log(LOG_ERROR) << "Could not link shaders; interface mismatch?";
        GLint logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<char> infoLog(logLength + 1);
        glGetProgramInfoLog(program, infoLog.size(), &logLength, infoLog.data());
        Log(LOG_ERROR) << "Error log: " << infoLog.data();
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteProgram(program);
        return false;
    }

    // Get all attribute/uniform locations

    attribs.g_Position = glGetAttribLocation(program, "g_Position");
    attribs.g_TexCoord0 = glGetAttribLocation(program, "g_TexCoord0");
    attribs.g_Tangent = glGetAttribLocation(program, "g_Tangent");
    attribs.g_Binormal = glGetAttribLocation(program, "g_Binormal");
    attribs.g_Normal = glGetAttribLocation(program, "g_Normal");

    uniforms.world = glGetUniformLocation(program, "world");
    uniforms.worldInverseTranspose = glGetUniformLocation(program, "worldInverseTranspose");
    uniforms.worldViewProj = glGetUniformLocation(program, "worldViewProj");
    uniforms.viewInverse = glGetUniformLocation(program, "viewInverse");
    uniforms.normalSampler = glGetUniformLocation(program, "normalSampler");
    uniforms.envSampler = glGetUniformLocation(program, "envSampler");

    return true;
}
