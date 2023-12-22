//
// Created by SeanR on 12/18/2023.
//

#include "pass.h"

#include "orthographic_camera.h"
#include "buffer_geometry.h"
#include "gl_render_target.h"
#include "gl_renderer.h"
#include "scene.h"
#include "material.h"
#include "mesh.h"

namespace threecpp_pass{
    Camera::sptr _camera = OrthographicCamera::create( - 1, 1, 1, - 1, 0, 1 ); // https://github.com/mrdoob/three.js/pull/21358


}

using namespace threecpp_pass;
namespace threecpp{
    FullScreenQuad::FullScreenQuad(Material::sptr material){
        _geometry = BufferGeometry::create();
        _geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create( { - 1, 3, 0, - 1, - 1, 0, 3, - 1, 0 }, 3 ));
        _geometry->setAttribute( AttributeName::uv, BufferAttribute<float>::create( { 0, 2, 0, 0, 2, 0 }, 2 ));

        _mesh = Mesh::create( _geometry, material );
    }

    void FullScreenQuad::render(std::shared_ptr<GLRenderer> renderer) {
        auto scene1 = std::make_shared<Scene>();
        //scene1->setBackgroundColor(Color().set(0xFF0000));
        Material::sptr mat = getMaterial();
        auto mesh = Mesh::create( _geometry, mat );
        scene1->add(mesh);
        _camera->updateProjectionMatrix();
        renderer->render( scene1, _camera );
    }

    Material::sptr FullScreenQuad::getMaterial() {
        return _mesh->material;
    }

    void FullScreenQuad::setMaterial(Material::sptr material) {
        _mesh->material = material;
    }

}
