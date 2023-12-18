//
// Created by SeanR on 2022/10/19.
//

#ifndef THREE_CPP_SRC_SCENES_SCENE_H
#define THREE_CPP_SRC_SCENES_SCENE_H

#include "object_3d.h"

#include "gl_cube_render_target.h"
#include "cube_texture.h"
#include "color.h"
#include "fog.h"
#include "material.h"

class Scene : public Object3D {
private:
    GLCubeRenderTarget::sptr cubeRenderTarget;
    Texture::sptr texture;
    CubeTexture::sptr cubeTexture;
    Color color;

public:
    using sptr = std::shared_ptr<Scene>;

    bool isColor = false;
    bool isCubeTexture = false;
    bool isGLCubeRenderTarget = false;
    bool isTexture = false;

    Fog::sptr fog = nullptr;

    Material::sptr overrideMaterial = nullptr;

    bool autoUpdate = true;

    Texture::sptr environment;

    //const void* background =nullptr;
    //const void* environment = nullptr;

    Scene() :Object3D() {
        type = "Scene";
    }

    virtual ~Scene() {}
    //{
    //	this->onDisposed.emitSignal(*this);
    //}
    Scene(const Scene& source) : Object3D(source) {
        type = "Scene";
        fog = source.fog;
        overrideMaterial = source.overrideMaterial;
        fog = source.fog;
        autoUpdate = source.autoUpdate;
    }
    virtual Scene* clone(bool recursive=true) override {
        return new Scene(*this);
    }

    virtual Scene& copy(const Scene& source, bool recursive = true) {
        Object3D::copy(source, recursive);

        fog = source.fog;
        overrideMaterial = source.overrideMaterial;
        fog = source.fog;
        autoUpdate = source.autoUpdate;

        return *this;
    }
    static sptr create() {
        return std::make_shared<Scene>();
    }

    Texture::sptr& getBackgroundTexture() {
        return texture;
    }
    CubeTexture::sptr& getBackgroundCubeTexture() {
        return cubeTexture;
    }

    GLCubeRenderTarget::sptr& getBackgroundCubeRenderTarget() {
        return cubeRenderTarget;
    }

    Color& getBackgroundColor() {
        return color;
    }

    void setBackgroundColor(Color& value) {
        color = value;
        isColor = true;
        isTexture = false;
        isCubeTexture = false;
        isGLCubeRenderTarget = false;
    }
    void setBackgroundTexture(Texture::sptr& value) {
        texture = value;
        isColor = false;
        isTexture = true;
        isCubeTexture = false;
        isGLCubeRenderTarget = false;
    }
    void setBackgroundCubeTexture(CubeTexture::sptr& value) {
        cubeTexture = value;
        isColor = false;
        isTexture = false;
        isCubeTexture = true;
        isGLCubeRenderTarget = false;
    }

    void setBackgroundCubeRenderTarget(GLCubeRenderTarget::sptr& value) {

        cubeRenderTarget = value;
        isColor = false;
        isTexture = false;
        isCubeTexture = false;
        isGLCubeRenderTarget = true;

    }

    bool hasBackground() const {
        return isColor || isTexture || isCubeTexture || isGLCubeRenderTarget;
    }

    threecpp::Signal<void(const Scene& scene)> onDisposed;

};

//struct Scene: public Object3D{
//    bool isScene = true;
//    std::string type = "Scene";
//    bool autoUpdate = true; // checked by the renderer
//
////    this.background = null;
//    std::shared_ptr<Color> background;
////    this.environment = null;
////    this.fog = null;
//    std::shared_ptr<Fog> fog;
//
////    this.overrideMaterial = null;
//    std::shared_ptr<Material> overrideMaterial;
//
//    Scene(std::shared_ptr<Color> background,std::shared_ptr<Fog> fog,std::shared_ptr<Material> overrideMaterial,bool autoUpdate)
//                                :background(background),fog(fog),overrideMaterial(overrideMaterial),autoUpdate(autoUpdate){}
//
//
//};
//
////copy( source, recursive ) {
////
////super.copy( source, recursive );
////
////if ( source.background !== null ) this.background = source.background.clone();
////if ( source.environment !== null ) this.environment = source.environment.clone();
////if ( source.fog !== null ) this.fog = source.fog.clone();
////
////if ( source.overrideMaterial !== null ) this.overrideMaterial = source.overrideMaterial.clone();
////
////this.autoUpdate = source.autoUpdate;
////this.matrixAutoUpdate = source.matrixAutoUpdate;
////
////return this;
////
////}
////
////toJSON( meta ) {
////
////    const data = super.toJSON( meta );
////
////    if ( this.fog !== null ) data.object.fog = this.fog.toJSON();
////
////    return data;
////
////}
////
////}



#endif //THREE_CPP_SRC_SCENES_SCENE_H
