#ifndef SRC_CAMERAS_PERSPECTIVE_CAMERA_H_
#define SRC_CAMERAS_PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "math_utils.h"

#include <string>
#include <memory>

class PerspectiveCamera : public Camera {
public:
    using sptr = std::shared_ptr<PerspectiveCamera>;

    View::sptr view;

    float focus = 10;

    float filmGauge = 35;

    float filmOffset = 0;

    PerspectiveCamera() : Camera() {
        isPerspective = true;
    }

    PerspectiveCamera(float fov, float aspect, float _near = std::numeric_limits<float>::quiet_NaN(), float _far=std::numeric_limits<float>::quiet_NaN()) : Camera(){
        isPerspective = true;
        this->fov = !fov ? fov : 50.0f;
        this->zoom = 1;
        this->_near = !std::isnan(_near) ? _near : 0.1f;
        this->_far = !std::isnan(_far) ? _far : 2000.0f;

        updateProjectionMatrix();
    }

    PerspectiveCamera(const PerspectiveCamera& camera) : Camera(camera) {
        isPerspective = true;
        //view = camera.view;
        focus = camera.focus;
        filmGauge = camera.filmGauge;
        filmOffset = camera.filmOffset;

        view = std::make_shared<View>();
        view->copy(*camera.view);

        updateProjectionMatrix();
    }

    virtual PerspectiveCamera* clone(){
        return new PerspectiveCamera(*this);
    }

    virtual PerspectiveCamera& copy(PerspectiveCamera& camera, bool recursive) {
        Camera::copy(camera, recursive);
        focus = camera.focus;
        filmGauge = camera.filmGauge;
        filmOffset = camera.filmOffset;

        view = std::make_shared<View>();
        view->copy(*camera.view);

        updateProjectionMatrix();

        return *this;
    }

    static sptr create(float fov, float aspect, float _near, float _far) {
        //return sptr(new PerspectiveCamera(fov, aspect, _near, _far));
        return std::make_shared<PerspectiveCamera>(fov, aspect, _near, _far);
    }

    virtual ~PerspectiveCamera() = default;

    void setFocalLength(float focalLength);

    float getFocalLength();

    float getEffectiveFOV();

    float getFilmWidth();

    float getFilmHeight();

    virtual void setViewOffset(float fullWidth, float fullHeight, float x, float y, float width, float height) override;

    virtual void clearViewOffset() override;

    virtual void updateProjectionMatrix() override;
};

class ArrayCamera : public PerspectiveCamera {
public:
    std::vector<Camera::sptr> cameras;

    ArrayCamera() : PerspectiveCamera() {
        isArrayCamera = true;
    }

    ArrayCamera(const std::vector<Camera::sptr>& cameras) : PerspectiveCamera(), cameras(cameras) {
        isArrayCamera = true;
    }

    ArrayCamera(const ArrayCamera& source) : PerspectiveCamera(source) {
        isArrayCamera = true;
        if (source.cameras.size() > 0) {
            cameras.resize(source.cameras.size());
            std::copy(source.cameras.begin(), source.cameras.end(), cameras.begin());
        }
    }
    using sptr = std::shared_ptr<ArrayCamera>;

    static sptr create(const std::vector<Camera::sptr>& cameras) {
        return sptr(new ArrayCamera(cameras));
    }

    ArrayCamera* clone() {
        return new ArrayCamera(*this);
    }

    ArrayCamera& copy(ArrayCamera& source,bool recursive) {
        PerspectiveCamera::copy(source, recursive);
        if (source.cameras.size() > 0) {
            cameras.resize(source.cameras.size());
            std::copy(source.cameras.begin(), source.cameras.end(), cameras.begin());
        }
        isArrayCamera = true;
        return *this;
    }

};

//using std::shared_ptr;
//class PerspectiveCamera : public Camera {
//    public:
//        string type{"PerspectiveCamera"};
//        double fov,zoom=1,near,far,focus=10,aspect,filmGauge,filmOffset;
//        shared_ptr<ViewOffset> view;
//
//    //public:
//        PerspectiveCamera(double fov = 50,double aspect = 1,double near = 0.1,double far = 2000)
//                                :fov(fov),zoom(1),near(near),far(far),focus(10),aspect(aspect),filmGauge(35),filmOffset(0){
//            updateProjectionMatrix();
//
//        }
//
//
//        /**
//         * Sets the FOV by focal length in respect to the current .filmGauge.
//         *
//         * The default film gauge is 35, so that the focal length can be specified for
//         * a 35mm (full frame) camera.
//         *
//         * Values for focal length and film gauge must have the same unit.
//         */
//        PerspectiveCamera& setFocalLength( double focalLength ) {
//
//            /** see {@link http://www.bobatkins.com/photography/technical/field_of_view.html} */
//            const double vExtentSlope = 0.5 * getFilmHeight() / focalLength;
//
//            fov = MathUtils::RAD2DEG * 2 * atan( vExtentSlope );
//            updateProjectionMatrix();
//
//            return *this;
//        }
//
//        /**
//         * Calculates the focal length from the current .fov and .filmGauge.
//         */
//        double getFocalLength() {
//            const double vExtentSlope = tan( MathUtils::DEG2RAD * 0.5 * fov );
//
//            return 0.5 * getFilmHeight() / vExtentSlope;
//        }
//
//        double getEffectiveFOV() {
//            return MathUtils::RAD2DEG * 2 * atan(
//                tan( MathUtils::DEG2RAD * 0.5 * fov ) / zoom );
//        }
//
//        double getFilmWidth() {
//            // film not completely covered in portrait format (aspect < 1)
//            return filmGauge * fmin( aspect, 1 );
//        }
//
//        double getFilmHeight() {
//            // film not completely covered in landscape format (aspect > 1)
//            return filmGauge / fmax( aspect, 1 );
//        }
//
//        /**
//         * Sets an offset in a larger frustum. This is useful for multi-window or
//         * multi-monitor/multi-machine setups.
//         *
//         * For example, if you have 3x2 monitors and each monitor is 1920x1080 and
//         * the monitors are in grid like this
//         *
//         *   +---+---+---+
//         *   | A | B | C |
//         *   +---+---+---+
//         *   | D | E | F |
//         *   +---+---+---+
//         *
//         * then for each monitor you would call it like this
//         *
//         *   const w = 1920;
//         *   const h = 1080;
//         *   const fullWidth = w * 3;
//         *   const fullHeight = h * 2;
//         *
//         *   --A--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 0, h * 0, w, h );
//         *   --B--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 1, h * 0, w, h );
//         *   --C--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 2, h * 0, w, h );
//         *   --D--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 0, h * 1, w, h );
//         *   --E--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 1, h * 1, w, h );
//         *   --F--
//         *   camera.setViewOffset( fullWidth, fullHeight, w * 2, h * 1, w, h );
//         *
//         *   Note there is no reason monitors have to be the same size or in a grid.
//         */
//        PerspectiveCamera& setViewOffset( double fullWidth, double fullHeight, double x, double y, double width, double height ) {
//            aspect = fullWidth / fullHeight;
//
//            if ( view == nullptr ) {
//                // this.view = {
//                //     enabled: true,
//                //     fullWidth: 1,
//                //     fullHeight: 1,
//                //     offsetX: 0,
//                //     offsetY: 0,
//                //     width: 1,
//                //     height: 1
//                // };
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
//        PerspectiveCamera& clearViewOffset() {
//            if ( view != nullptr ) {
//                view->enabled = false;
//            }
//
//            updateProjectionMatrix();
//
//            return *this;
//        }
//
//        PerspectiveCamera& updateProjectionMatrix() {
//            const double near = this->near;
//            double top = near * tan( MathUtils::DEG2RAD * 0.5 * fov ) / zoom;
//            double height = 2 * top;
//            double width = aspect * height;
//            double left = - 0.5 * width;
//
//            if ( view != nullptr && view->enabled ) {
//                const double fullWidth = view->fullWidth,
//                    fullHeight = view->fullHeight;
//
//                left += view->offsetX * width / fullWidth;
//                top -= view->offsetY * height / fullHeight;
//                width *= view->width / fullWidth;
//                height *= view->height / fullHeight;
//
//            }
//
//            const double skew = filmOffset;
//            if ( skew != 0 ) left += near * skew / getFilmWidth();
//
//            projectionMatrix->makePerspective( left, left + width, top, top - height, near, far );
//            projectionMatrixInverse->copy( *projectionMatrix ).invert();
//
//            return *this;
//        }
//
//	// toJSON( meta ) {
//
//	// 	const data = super.toJSON( meta );
//
//	// 	data.object.fov = this.fov;
//	// 	data.object.zoom = this.zoom;
//
//	// 	data.object.near = this.near;
//	// 	data.object.far = this.far;
//	// 	data.object.focus = this.focus;
//
//	// 	data.object.aspect = this.aspect;
//
//	// 	if ( this.view !== null ) data.object.view = Object.assign( {}, this.view );
//
//	// 	data.object.filmGauge = this.filmGauge;
//	// 	data.object.filmOffset = this.filmOffset;
//
//	// 	return data;
//
//	// }
//
//};


#endif /* SRC_CAMERAS_PERSPECTIVE_CAMERA_H_ */
