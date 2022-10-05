#ifndef SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_
#define SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"

#include <string>
#include <memory>

using std::string;
using std::shared_ptr;
class OrthographicCamera : Camera {
    public:
        string type{"OrthographicCamera"};

        double zoom,left,right,top,bottom,near,far;
        shared_ptr<ViewOffset> view;

        OrthographicCamera(double left = -1,double right = 1,double top=1,double bottom = -1,double near = 0.1,double far = 2000)
                    :left(left),right(right),top(top),bottom(bottom),near(near),far(far),zoom(1),view(nullptr){
                        updateProjectionMatrix();
                    }



	// copy( source, recursive ) {

	// 	super.copy( source, recursive );

	// 	this.left = source.left;
	// 	this.right = source.right;
	// 	this.top = source.top;
	// 	this.bottom = source.bottom;
	// 	this.near = source.near;
	// 	this.far = source.far;

	// 	this.zoom = source.zoom;
	// 	this.view = source.view === null ? null : Object.assign( {}, source.view );

	// 	return this;

	// }

        OrthographicCamera& setViewOffset( double fullWidth, double fullHeight, double x, double y, double width, double height ) {

            if ( view == nullptr ) {
                view = std::make_shared<ViewOffset>(ViewOffset{true,1,1,0,0,1,1});
            }

            view->enabled = true;
            view->fullWidth = fullWidth;
            view->fullHeight = fullHeight;
            view->offsetX = x;
            view->offsetY = y;
            view->width = width;
            view->height = height;

            updateProjectionMatrix();

            return *this;
        }

        OrthographicCamera& clearViewOffset() {
            if ( view != nullptr ) {
                view->enabled = false;
            }

            updateProjectionMatrix();

            return *this;
        }

        OrthographicCamera& updateProjectionMatrix() {
            const double dx = ( right - left ) / ( 2 * zoom );
            const double dy = ( top - bottom ) / ( 2 * zoom );
            const double cx = ( right + left ) / 2;
            const double cy = ( top + bottom ) / 2;

            double left = cx - dx;
            double right = cx + dx;
            double top = cy + dy;
            double bottom = cy - dy;

            if ( view != nullptr && view->enabled ) {

                const double scaleW = ( right - left ) / view->fullWidth / zoom;
                const double scaleH = ( top - bottom ) / view->fullHeight / zoom;

                left += scaleW * view->offsetX;
                right = left + scaleW * view->width;
                top -= scaleH * view->offsetY;
                bottom = top - scaleH * view->height;

            }

            projectionMatrix->makeOrthographic( left, right, top, bottom, near, far );
            projectionMatrixInverse->copy( *projectionMatrix ).invert();

            return *this;
        }

	// toJSON( meta ) {

	// 	const data = super.toJSON( meta );

	// 	data.object.zoom = this.zoom;
	// 	data.object.left = this.left;
	// 	data.object.right = this.right;
	// 	data.object.top = this.top;
	// 	data.object.bottom = this.bottom;
	// 	data.object.near = this.near;
	// 	data.object.far = this.far;

	// 	if ( this.view !== null ) data.object.view = Object.assign( {}, this.view );

	// 	return data;

	// }

};


#endif /* SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_ */
