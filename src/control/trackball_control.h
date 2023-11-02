//
// Created by SeanR on 2023/10/20.
//

#ifndef THREE_CPP_TRACKBALL_CONTROL_H
#define THREE_CPP_TRACKBALL_CONTROL_H

#include "vector2.h"
#include "vector3.h"
#include "vector4.h"
#include "camera.h"

#include "common_types.h"

namespace control {
    enum class STATE : int {
        NONE = -1,
        ROTATE = 0,
        ZOOM = 1,
        PAN = 2,
        TOUCH_ROTATE = 3,
        TOUCH_ZOOM_PAN = 4
    };

    class TrackballControls {
    private:
        //{ 65 /*A*/, 83 /*S*/, 68 /*D*/ };
        threecpp::byte keys[3] = {65, 83, 68};
        // internal variables
        Vector3 target;

        Vector3 lastPosition;

        float lastZoom = 1;

        STATE state = STATE::NONE;

        STATE keyState = STATE::NONE;

        Vector3 eye;

        Vector3 rotateStart;

        Vector3 rotateEnd;

        Vector2 zoomStart;

        Vector2 zoomEnd;

        float touchZoomDistanceStart = 0.0f;

        float touchZoomDistanceEnd = 0.0f;

        Vector2 panStart;

        Vector2 panEnd;

        Vector4 screen;

        Vector3 target0;

        Vector3 position0;

        Vector3 up0;

        float zoom0;
    public:
        using sptr = std::shared_ptr<TrackballControls>;

        bool enabled = true;

        float rotateSpeed = 1.0f;
        float zoomSpeed = 1.2f;
        float panSpeed = 0.3f;

        bool noRotate = false;
        bool noZoom = false;
        bool noPan = false;
        bool noRoll = false;

        bool staticMoving = false;
        float dynamicDampingFactor = 0.2f;

        float minDistance = 0.0f;
        float maxDistance = std::numeric_limits<float>::infinity();


        Camera::sptr camera;

        TrackballControls() {
            target.set(0, 0, 0);

            lastPosition.set(0, 0, 0);

            eye.set(0, 0, 0);

            rotateStart.set(0, 0, 0);

            rotateEnd.set(0, 0, 0);

            zoomStart.set(0, 0);

            zoomEnd.set(0, 0);

            panStart.set(0, 0);

            panEnd.set(0, 0);

            screen.set(0, 0, 0, 0);

            target0.set(0, 0, 0);

            position0.set(0, 0, 0);

            up0.set(0, 1, 0);

            zoom0 = 1;
        }

        TrackballControls(const Camera::sptr &camera, const Vector4 &screen) : TrackballControls() {
            this->camera = camera;
            this->screen = screen;
            target0.copy(target);
            position0.copy(camera->position);
            up0.copy(camera->up);
            zoom0 = camera->zoom;
        }

        static sptr create() {
            return sptr(new TrackballControls());
        }

        static sptr create(const Camera::sptr &camera, const Vector4 &screen) {
            return sptr(new TrackballControls(camera, screen));
        }

        void initTrackballControls(const Camera::sptr &camera, const Vector4 &screen) {

            target.set(0, 0, 0);
            lastPosition.set(0, 0, 0);
            eye.set(0, 0, 0);
            rotateStart.set(0, 0, 0);
            rotateEnd.set(0, 0, 0);
            zoomStart.set(0, 0);
            zoomEnd.set(0, 0);
            panStart.set(0, 0);
            panEnd.set(0, 0);

            this->camera = camera;
            this->screen = screen;
            target0.copy(target);
            position0.copy(camera->position);
            up0.copy(camera->up);
            zoom0 = camera->zoom;

        }

        Vector2 getMouseOnScreen(float pageX, float pageY);

        Vector3 getMouseProjectionOnBall(float pageX, float pageY);

        void rotateCamera();

        void zoomCamera();

        void panCamera();

        void checkDistance();

        void update();

        void sizeChanged(const Vector4 &screen);

        void reset();

        void keydown(threecpp::byte keyCode);

        void keyup();

        void mouseDown(unsigned button, float x, float y);

        void mouseMove(float x, float y);

        void mouseUp();

        void mouseWheel(float delta);

        void touchstart();

        void touchmove();

        void touchend();

        void setState(int mouseState) {
            this->state = (STATE) mouseState;
        }
    };
}

#endif //THREE_CPP_TRACKBALL_CONTROL_H
