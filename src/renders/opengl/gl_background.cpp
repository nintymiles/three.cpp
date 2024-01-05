//
// Created by SeanR on 2023/10/18.
//

#include "gl_background.h"

#include "scene.h"
#include "cube_texture.h"
#include "camera.h"
#include "box_geometry.h"
#include "plane_geometry.h"
#include "shader_material.h"
#include "shader_lib.h"
#include "texture.h"
#include "gl_renderer.h"
#include "gl_render_target.h"
#include "gl_cube_maps.h"

#include "shader_chunk.h"

#include <memory>

void GLBackground::setClear(const Color& color, float alpha){
    state->colorBuffer.setClear(color.r, color.g, color.b, alpha, premultipliedAlpha);
}

Color& GLBackground::getClearColor(){
    return clearColor;
}

void GLBackground::setClearColor(const Color& color, const float alpha){
    clearColor.copy(color);
    clearAlpha = alpha;

    setClear(clearColor, clearAlpha);

}

float GLBackground::getClearAlpha(){
    return clearAlpha;
}

void GLBackground::setClearAlpha(float alpha){
    clearAlpha = alpha;
    setClear(clearColor, clearAlpha);
}

void GLBackground::render(GLRenderer& renderer,GLCubeMaps* cubeMaps, GLRenderList& renderList,Scene& scene, Camera& camera, bool forceClear){
    if (!scene.hasBackground()) {
        setClear(clearColor, clearAlpha);
    }
    else if (scene.isColor) {
        Color color = scene.getBackgroundColor();
        setClear(color, 1);
        forceClear = true;
    }

    auto background = scene.getBackgroundTexture() ? scene.getBackgroundTexture() : scene.getBackgroundCubeTexture();
    auto tex = cubeMaps->get(background);

    if (renderer.autoClear || forceClear)
        renderer.clear(renderer.autoClearColor, renderer.autoClearDepth, renderer.autoClearStencil);

    //todo: optimize here
    //if (scene.isCubeTexture || scene.isGLCubeRenderTarget || (scene.getBackgroundCubeTexture()!=nullptr && scene.getBackgroundCubeTexture()->mapping == TextureMapping::CubeReflectionMapping) || (scene.getBackgroundTexture()!=nullptr && scene.getBackgroundTexture()->mapping == TextureMapping::CubeReflectionMapping)) {
    if(tex && tex->isCubeTexture){
        if (boxMesh == nullptr) {
            boxMesh = Mesh::create(std::make_shared<BoxGeometry>(1, 1, 1));
            ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();
            shaderMaterial->type = "BackgroundCubeMaterial";
            shaderMaterial->uniforms = std::make_shared<UniformValues>(getShader("cube").uniforms);
            shaderMaterial->vertexShader = getShader("cube").vertexShader;
            shaderMaterial->fragmentShader = getShader("cube").fragmentShader;
            shaderMaterial->side = Side::BackSide;
            shaderMaterial->depthTest = false;
            shaderMaterial->depthWrite = false;
            shaderMaterial->fog = false;
            boxMesh->material = shaderMaterial;

            BoxGeometry::sptr geometry = std::dynamic_pointer_cast<BoxGeometry>(boxMesh->geometry);
            geometry->deleteAttribute(AttributeName::normal);
            geometry->deleteAttribute(AttributeName::uv);
            //geometry->normal.reset();// geometry->deleteAttribute<float>("normal");
            //geometry->uv.reset();// geometry->deleteAttribute<float>("uv");

            //boxMesh->onBeforeRender.connect(*this, &GLBackground::beforeRender);
            boxMesh->onBeforeRender.connect(*this,&GLBackground::beforeRender);

            shaderMaterial->envMap = tex;//(*shaderMaterial->uniforms)["envMap"].get<Texture::sptr>();//->get<Texture::ptr>("envMap");
            objects->update(*boxMesh);
        }
        Texture::sptr emptyTexture;
        //todo:fix here
        Texture::sptr texture = scene.getBackgroundCubeTexture();//scene.isGLCubeRenderTarget ? scene.getBackgroundCubeRenderTarget()->texture : emptyTexture;

        ShaderMaterial::sptr material = std::dynamic_pointer_cast<ShaderMaterial>(boxMesh->material);
        material->uniforms->set("envMap", texture);

        if (scene.isCubeTexture)
            material->uniforms->set("flipEnvMap", -1);
        else
            material->uniforms->set("flipEnvMap", 1);

        if ((scene.isColor && !((Color*)currentBackground)->equals(scene.getBackgroundColor()))
            || (texture!=nullptr && texture->version != currentBackgroundVersion)
            || (currentTonemapping != renderer.toneMapping)) {
            material->needsUpdate = true;

            currentBackground = (void *)&scene.getBackgroundColor();

            if (texture!=nullptr)
                currentBackgroundVersion = texture->version;

            currentTonemapping = renderer.toneMapping;
        }
        renderList.unshift(boxMesh, std::dynamic_pointer_cast<BufferGeometry>(boxMesh->geometry), boxMesh->material, 0, 0, nullptr);
    } else if (scene.isTexture) { //scene background is texture
        if (planeMesh == nullptr) {
            PlaneGeometry::sptr geometry = std::make_shared<PlaneGeometry>(2, 2);
            planeMesh = std::make_shared<Mesh>(geometry);
            ShaderMaterial::sptr shaderMaterial = ShaderMaterial::create();
            shaderMaterial->type = "BackgroundMaterial";
            shaderMaterial->uniforms = std::make_shared<UniformValues>(getShader("background").uniforms);
            shaderMaterial->vertexShader = getShader("background").vertexShader;
            shaderMaterial->fragmentShader = getShader("background").fragmentShader;
            shaderMaterial->side = Side::FrontSide;
            shaderMaterial->depthTest = false;
            shaderMaterial->depthWrite = false;
            shaderMaterial->fog = false;
            planeMesh->material = shaderMaterial;

            geometry->deleteAttribute(AttributeName::normal);
            //geometry->normal.reset();// geometry->deleteAttribute<float>("normal");

            shaderMaterial->map = (*shaderMaterial->uniforms)["t2D"].get<Texture::sptr>();
            objects->update(*planeMesh);
        }
        ShaderMaterial::sptr material = std::dynamic_pointer_cast<ShaderMaterial>(planeMesh->material);
        material->uniforms->set("t2D", scene.getBackgroundTexture()); //-->shared ptr

        if (scene.getBackgroundTexture()->matrixAutoUpdate) {
            scene.getBackgroundTexture()->updateMatrix();
        }
        material->uniforms->set("uvTransform", scene.getBackgroundTexture()->matrix);
        if ((Texture*)currentBackground != scene.getBackgroundTexture().get() ||
            currentBackgroundVersion != scene.getBackgroundTexture()->version ||
            currentTonemapping != renderer.toneMapping) {

            material->needsUpdate = true;

            //fixme: refactor this shared_ptr to naked_ptr
            currentBackground = (void *)scene.getBackgroundTexture().get();
            currentBackgroundVersion = scene.getBackgroundTexture()->version;
            currentTonemapping = renderer.toneMapping;
        }

        renderList.unshift(planeMesh, std::dynamic_pointer_cast<BufferGeometry>(planeMesh->geometry), planeMesh->material, 0, 0, nullptr);
    }

}

void GLBackground::beforeRender(GLRenderer& renderer, const std::shared_ptr<Scene>& scene, const std::shared_ptr<Camera>& camera, const std::shared_ptr<Object3D>& object,const std::shared_ptr<BufferGeometry>& geometry, const std::shared_ptr<Material>& material,
                                const std::shared_ptr<GLRenderTarget>& renderTarget, threecpp::DrawRange* group){
    object->matrixWorld.copyPosition(camera->matrixWorld);
}