//
// Created by SeanR on 2023/11/3.
//

#include "gl_loader_obj_mtl.h"

#include "obj_loader.h"
#include "common_utils.h"

void GLLoaderObjMtl::loadObj(const Material::sptr& material,std::string path,Group::sptr& objGrp){
    OBJLoader loader;

    objGrp = loader.load(threecpp::getProgramPath() + "\\" + path);


    objGrp->traverse([&](Object3D& o) {
        o.material = meshMaterial;
        if (instanceOf<Mesh>(&o) && o.materials.size() > 1) {
            int size = o.materials.size();
            for (int i = 0; i < size; i++)
                o.materials.push_back(meshMaterial);
        }
    });
    objGrp->scale.set(4, 4, 4);
}

void GLLoaderObjMtl::render(){
    Vector2 mousePos = controller->getMousePos();
    camera->position.x += ( mousePos.x - camera->position.x ) * .05;
    camera->position.y += ( mousePos.y - camera->position.y ) * .05;

    //std::cout << "mouse.x:"<< mousePos.x << "|| mouse.y:" << mousePos.y << std::endl;
    camera->lookAt( scene->position );

    ApplicationBase::render();
}
