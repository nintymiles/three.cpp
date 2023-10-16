#ifndef SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_
#define SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"

#include <string>
#include <memory>

class OrthographicCamera : public Camera {
public:
    CameraView view;

    using sptr = std::shared_ptr<OrthographicCamera>;

    OrthographicCamera(float left=std::numeric_limits<float>::quiet_NaN(), float right= std::numeric_limits<float>::quiet_NaN(), float top= std::numeric_limits<float>::quiet_NaN(), float bottom= std::numeric_limits<float>::quiet_NaN(), float _near= std::numeric_limits<float>::quiet_NaN(), float _far= std::numeric_limits<float>::quiet_NaN()) : Camera() {

        type = "OrthographicCamera";
        isOrthographic = true;

        zoom = 1;

        this->left = !std::isnan(left)  ? left : -1;
        this->cameraRight = !std::isnan(right)  ? right : 1;
        this->top = !std::isnan(top)  ? top : 1;
        this->bottom = !std::isnan(bottom)  ? bottom : -1;

        this->_near = std::isnan(_near)  ? _near : 0.1f;
        this->_far = std::isnan(_far)  ? _far : 2000;

        view.enabled = false;

        this->updateProjectionMatrix();

    }
    OrthographicCamera(OrthographicCamera& source) : Camera(source) {
        this->zoom = source.zoom;
        isOrthographic = true;
        this->left = source.left;
        this->cameraRight = source.cameraRight;
        this->top = source.top;
        this->bottom = source.bottom;
        this->view = source.view;
    }

    OrthographicCamera& copy(OrthographicCamera& source) {
        Camera::copy(source);
        this->zoom = source.zoom;

        this->left = source.left;
        this->cameraRight = source.cameraRight;
        this->top = source.top;
        this->bottom = source.bottom;
        this->view = source.view;
    }


    static ptr create(float left = std::numeric_limits<float>::quiet_NaN(), float right = std::numeric_limits<float>::quiet_NaN(), float top = std::numeric_limits<float>::quiet_NaN(), float bottom = std::numeric_limits<float>::quiet_NaN(), float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()) {
        return ptr(new OrthographicCamera(left, right, top, bottom, _near, _far));
    }

    void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) override {
        view.enabled = true;
        view.fullWidth = fullWidth;
        view.fullHeight = fullHeight;
        view.offsetX = x;
        view.offsetY = y;
        view.width = width;
        view.height = height;

        this->updateProjectionMatrix();
    }
    void clearViewOffset() override {
        this->view.enabled = false;

        this->updateProjectionMatrix();
    }
    virtual OrthographicCamera* clone() {
        return new OrthographicCamera(*this);
    }
    virtual void updateProjectionMatrix() override {
        auto dx = (this->cameraRight - this->left) / (2 * this->zoom);
        auto dy = (this->top - this->bottom) / (2 * this->zoom);
        auto cx = (this->cameraRight + this->left) / 2;
        auto cy = (this->top + this->bottom) / 2;

        auto left = cx - dx;
        auto right = cx + dx;
        auto top = cy + dy;
        auto bottom = cy - dy;

        if (this->view.enabled) {

            auto zoomW = this->zoom / (this->view.width / this->view.fullWidth);
            auto zoomH = this->zoom / (this->view.height / this->view.fullHeight);
            auto scaleW = (this->cameraRight - this->left) / this->view.width;
            auto scaleH = (this->top - this->bottom) / this->view.height;

            left += scaleW * (this->view.offsetX / zoomW);
            right = left + scaleW * (this->view.width / zoomW);
            top -= scaleH * (this->view.offsetY / zoomH);
            bottom = top - scaleH * (this->view.height / zoomH);

        }

        this->projectionMatrix = Matrix4().makeOrthographic(left, right, top, bottom, this->_near, this->_far);

        this->projectionMatrixInverse.getInverse(this->projectionMatrix);
    }
};

//using std::string;
//using std::shared_ptr;
//class OrthographicCamera : public Camera {
//    public:
//        string type{"OrthographicCamera"};
//
//        double zoom,left,right,top,bottom,near,far;
//        shared_ptr<ViewOffset> view;
//
//        OrthographicCamera(double left = -1,double right = 1,double top=1,double bottom = -1,double near = 0.1,double far = 2000)
//                    :left(left),right(right),top(top),bottom(bottom),near(near),far(far),zoom(1),view(nullptr){
//                        updateProjectionMatrix();
//                    }
//
//
//
//	// copy( source, recursive ) {
//
//	// 	super.copy( source, recursive );
//
//	// 	this.left = source.left;
//	// 	this.right = source.right;
//	// 	this.top = source.top;
//	// 	this.bottom = source.bottom;
//	// 	this.near = source.near;
//	// 	this.far = source.far;
//
//	// 	this.zoom = source.zoom;
//	// 	this.view = source.view === null ? null : Object.assign( {}, source.view );
//
//	// 	return this;
//
//	// }
//
//        OrthographicCamera& setViewOffset( double fullWidth, double fullHeight, double x, double y, double width, double height ) {
//
//            if ( view == nullptr ) {
//                view = std::make_shared<ViewOffset>(ViewOffset{true,1,1,0,0,1,1});
//            }
//
//            view->enabled = true;
//            view->fullWidth = fullWidth;
//            view->fullHeight = fullHeight;
//            view->offsetX = x;
//            view->offsetY = y;
//            view->width = width;
//            view->height = height;
//
//            updateProjectionMatrix();
//
//            return *this;
//        }
//
//        OrthographicCamera& clearViewOffset() {
//            if ( view != nullptr ) {
//                view->enabled = false;
//            }
//
//            updateProjectionMatrix();
//
//            return *this;
//        }
//
//        OrthographicCamera& updateProjectionMatrix() {
//            const double dx = ( right - left ) / ( 2 * zoom );
//            const double dy = ( top - bottom ) / ( 2 * zoom );
//            const double cx = ( right + left ) / 2;
//            const double cy = ( top + bottom ) / 2;
//
//            double left = cx - dx;
//            double right = cx + dx;
//            double top = cy + dy;
//            double bottom = cy - dy;
//
//            if ( view != nullptr && view->enabled ) {
//
//                const double scaleW = ( right - left ) / view->fullWidth / zoom;
//                const double scaleH = ( top - bottom ) / view->fullHeight / zoom;
//
//                left += scaleW * view->offsetX;
//                right = left + scaleW * view->width;
//                top -= scaleH * view->offsetY;
//                bottom = top - scaleH * view->height;
//
//            }
//
//            projectionMatrix->makeOrthographic( left, right, top, bottom, near, far );
//            projectionMatrixInverse->copy( *projectionMatrix ).invert();
//
//            return *this;
//        }
//
//	// toJSON( meta ) {
//
//	// 	const data = super.toJSON( meta );
//
//	// 	data.object.zoom = this.zoom;
//	// 	data.object.left = this.left;
//	// 	data.object.right = this.right;
//	// 	data.object.top = this.top;
//	// 	data.object.bottom = this.bottom;
//	// 	data.object.near = this.near;
//	// 	data.object.far = this.far;
//
//	// 	if ( this.view !== null ) data.object.view = Object.assign( {}, this.view );
//
//	// 	return data;
//
//	// }
//
//};


#endif /* SRC_CAMERAS_ORTHOGRAPHIC_CAMERA_H_ */
