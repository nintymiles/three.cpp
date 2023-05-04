#ifndef OPENGL_INDEXED_BUFFER_RENDER_H
#define OPENGL_INDEXED_BUFFER_RENDER_H

#include<GLES3/gl3.h>

struct IndexValue{
    GLenum type;
    int bytesPerElement;
};

class OpenGLIndexedBufferRenderer{
private:
    GLenum mode;
    GLenum type;
    int bytesPerElement;

public:
    OpenGLIndexedBufferRenderer& setMode(GLenum modeVal){
        mode = modeVal;
        return *this;
    }

    OpenGLIndexedBufferRenderer& setIndex(IndexValue& indexValue){
        type = indexValue.type;
        bytesPerElement = indexValue.bytesPerElement;
        return *this;
    }

    OpenGLIndexedBufferRenderer& render(int start,int count){
        glDrawElements(mode,count,GL_UNSIGNED_BYTE, nullptr);
        return *this;
    }


    OpenGLIndexedBufferRenderer& renderInstances(int start,int count,int instanceCount){
        glDrawElementsInstanced(mode,count,GL_UNSIGNED_BYTE, nullptr,instanceCount);
        return *this;
    }

};


// 	function renderInstances( start, count, primcount ) {
// 		if ( primcount === 0 ) return;
// 		let extension, methodName;
// 		if ( isWebGL2 ) {
// 			extension = gl;
// 			methodName = 'drawElementsInstanced';
// 		} else {
// 			extension = extensions.get( 'ANGLE_instanced_arrays' );
// 			methodName = 'drawElementsInstancedANGLE';
// 			if ( extension === null ) {
// 				console.error( 'THREE.WebGLIndexedBufferRenderer: using THREE.InstancedBufferGeometry but hardware does not support extension ANGLE_instanced_arrays.' );
// 				return;
// 			}
// 		}
// 		extension[ methodName ]( mode, count, type, start * bytesPerElement, primcount );
// 		info.update( count, mode, primcount );
// 	}

#endif /* OPENGL_INDEXED_BUFFER_RENDER_H */
