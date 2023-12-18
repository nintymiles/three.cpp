//
// Created by SeanR on 12/18/2023.
//

#include "pass.h"

#include "orthographic_camera.h"
#include "buffer_geometry.h"

#include "gl_renderer.h"
#include "scene.h"

namespace threecpp_pass{
    Camera::sptr _camera = OrthographicCamera::create( - 1, 1, 1, - 1, 0, 1 ); // https://github.com/mrdoob/three.js/pull/21358

    BufferGeometry::sptr _geometry = BufferGeometry::create();
    
}

using namespace threecpp_pass;
namespace threecpp{
    FullScreenQuad::FullScreenQuad(Material::sptr material){
        _geometry->setAttribute( AttributeName::position, BufferAttribute<float>::create( { - 1, 3, 0, - 1, - 1, 0, 3, - 1, 0 }, 3 ));
        _geometry->setAttribute( AttributeName::uv, BufferAttribute<float>::create( { 0, 2, 0, 0, 2, 0 }, 2 ));

        _mesh = Mesh::create( _geometry, material );
    }

    void FullScreenQuad::render(std::shared_ptr<GLRenderer> renderer) {
        Scene::sptr scene1 = Scene::create();
        scene1->add(_mesh);
        renderer->render( scene1, _camera );
    }

    Material::sptr FullScreenQuad::getMaterial() {
        return _mesh->material;
    }

    void FullScreenQuad::setMaterial(Material::sptr material) {
        _mesh->material = material;
    }

}
