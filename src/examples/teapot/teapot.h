//
// Created by sf on 8/9/17.
//

#ifndef IMGUI_DEMO_TEAPOT_H
#define IMGUI_DEMO_TEAPOT_H

#include <EGL/egl.h>
#include <GLES3/gl3.h>

#include "vector3.h"
#include "matrix4.h"

class Teapot {
public:
    Teapot();
    ~Teapot();
    bool init();
    void draw();
    void rotateBy(float angleX, float angleY);
    void rotateTo(float angleX);
    void rotateCameraBy(float angleX, float angleY);
    void rotateCameraTo(float angleX);
    void zoomBy(float zoomFactor);
    float zoomValue();

private:
    struct vtxData {
        Vector3 pos;
        Vector3 norm;
        Vector3 tangent;
        Vector3 binorm;
        Vector3 texcoord;
    };

    struct uniformData {
        Matrix4 world;
        Matrix4 worldInverseTranspose;
        Matrix4 worldViewProj;
        Matrix4 viewInverse;
    } unfData;


    static GLuint g_vao;
    int num_vertices;
    int num_indices;
    GLuint ibo;
    GLuint vbo;
    GLuint tex_skybox;
    GLuint tex_bump;

    struct {
        GLint g_Position;
        GLint g_TexCoord0;
        GLint g_Tangent;
        GLint g_Binormal;
        GLint g_Normal;
    } attribs;

    struct {
        GLint world;
        GLint worldInverseTranspose;
        GLint worldViewProj;
        GLint viewInverse;
        GLint normalSampler;
        GLint envSampler;
    } uniforms;
    GLuint program;

    GLfloat rotX, rotY, zoom;
    GLfloat camRX, camRY;
    GLfloat addRotX, addRotY, addZoom;
    GLfloat addCamRX, addCamRY;

    bool compileShaders();

};

#endif //IMGUI_DEMO_TEAPOT_H
