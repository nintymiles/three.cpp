//
// Created by SeanR on 11/22/2023.
//

#ifndef THREE_CPP_ORBIT_CONTROL_H
#define THREE_CPP_ORBIT_CONTROL_H

// Unlike TrackballControls, it maintains the "up" direction object.up (+Y by default).
//
//    Orbit - left mouse / touch: one-finger move
//    Zoom - middle mouse, or mousewheel / touch: two-finger spread or squish
//    Pan - right mouse, or left mouse + ctrl/meta/shiftKey, or arrow keys / touch: two-finger move

#include "viewport_control.h"
#include "object_3d.h"
#include "vector3.h"
#include "vector2.h"
#include "vector4.h"
#include "number.h"
#include "spherical.h"
#include "math_utils.h"

#include "camera.h"

#include <cmath>

class OrbitControl:public ViewPortControl{
    //
    // internals
    //
    //{ 65 /*A*/, 83 /*S*/, 68 /*D*/ };
    threecpp::byte keys[3] = {65, 83, 68};
    enum STATE {
        NONE = - 1,
        ROTATE = 0,
        DOLLY = 1,
        PAN = 2,
        TOUCH_ROTATE = 3,
        TOUCH_PAN = 4,
        TOUCH_DOLLY_PAN = 5,
        TOUCH_DOLLY_ROTATE = 6
    };
    enum MouseButton{
        LEFT = 0,
        MIDDLE = 1,
        RIGHT = 2,
    };

    Vector3 lastPosition;
    Quaternion lastQuaternion;
    float lastZoom = 1;

    STATE state = STATE::NONE;
    STATE keyState = STATE::NONE;
    MouseButton mouseAction = MouseButton::LEFT;

    const float EPS = math_number::EPSILON;

    // current position in spherical coordinates
    Spherical spherical;
    Spherical sphericalDelta;

    float scale = 1;
    Vector3 panOffset;
    bool zoomChanged = false;

    Vector2 rotateStart;
    Vector2 rotateEnd;
    Vector2 rotateDelta;

    Vector2 panStart;
    Vector2 panEnd;
    Vector2 panDelta;

    Vector2 dollyStart;
    Vector2 dollyEnd;
    Vector2 dollyDelta;

    //control whether move works
    bool noRotate = false;
    bool noZoom = false;
    bool noPan = false;
    bool noRoll = false;
    Vector3 eye;

    //const pointers = [];
    //const pointerPositions = {};
    Vector4 screen;
    Vector2 mousePos;

    Vector3 target0;
    Vector3 position0;
    Vector3 up0;
    float zoom0;

public:
    using sptr = std::shared_ptr<OrbitControl>;

    //Object3D::sptr object;

    Camera::sptr camera;

    // Set to false to disable this control
    bool enabled = true;

    // "target" sets the location of focus, where the object orbits around
    Vector3 target;

    // How far you can dolly in and out ( PerspectiveCamera only )
    float minDistance = 0;
    float maxDistance = std::numeric_limits<float>::infinity();

    // How far you can zoom in and out ( OrthographicCamera only )
    float minZoom = 0;
    float maxZoom = std::numeric_limits<float>::infinity();

    // How far you can orbit vertically, upper and lower limits.
    // Range is 0 to Math.PI radians.
    float minPolarAngle = 0; // radians
    float maxPolarAngle = math_number::PI; // radians

    // How far you can orbit horizontally, upper and lower limits.
    // If set, the interval [ min, max ] must be a sub-interval of [ - 2 PI, 2 PI ], with ( max - min < 2 PI )
    float minAzimuthAngle = - std::numeric_limits<float>::infinity(); // radians
    float maxAzimuthAngle = std::numeric_limits<float>::infinity(); // radians

    // Set to true to enable damping (inertia)
    // If damping is enabled, you must call controls.update() in your animation loop
    bool enableDamping = false;
    float dampingFactor = 0.05;

    // This option actually enables dollying in and out; left as "zoom" for backwards compatibility.
    // Set to false to disable zooming
    bool enableZoom = true;
    float zoomSpeed = 1.0;

    // Set to false to disable rotating
    bool enableRotate = true;
    float rotateSpeed = 1.0;

    // Set to false to disable panning
    bool enablePan = true;
    float panSpeed = 1.0;
    bool screenSpacePanning = true; // if false, pan orthogonal to world-space direction camera.up
    float keyPanSpeed = 7.0;	// pixels moved per arrow key push

    // Set to true to automatically rotate around the target
    // If auto-rotate is enabled, you must call controls.update() in your animation loop
    bool autoRotate = false;
    float autoRotateSpeed = 2.0; // 30 seconds per orbit when fps is 60

    // The four arrow keys
    //this.keys = { LEFT: 'ArrowLeft', UP: 'ArrowUp', RIGHT: 'ArrowRight', BOTTOM: 'ArrowDown' };

    // Mouse buttons
    //this.mouseButtons = { LEFT: MOUSE.ROTATE, MIDDLE: MOUSE.DOLLY, RIGHT: MOUSE.PAN };

    // Touch fingers
    //this.touches = { ONE: TOUCH.ROTATE, TWO: TOUCH.DOLLY_PAN };

    // for reset
    //this.target0 = this.target.clone();
    //this.position0 = this.object.position.clone();
    //this.zoom0 = this.object.zoom;

    // the target DOM element for key events
    //this._domElementKeyEvents = null;

public:

    OrbitControl() {
        target.set(0, 0, 0);

        lastPosition.set(0, 0, 0);
        lastQuaternion.set(0,0,0,0);
//
//        eye.set(0, 0, 0);
//
        rotateStart.set(0, 0);

        rotateEnd.set(0, 0);

        dollyStart.set(0, 0);

        dollyEnd.set(0, 0);

        panStart.set(0, 0);

        panEnd.set(0, 0);

        screen.set(0, 0, 0, 0);

        target0.set(0, 0, 0);

        position0.set(0, 0, 0);

        up0.set(0, 1, 0);

        zoom0 = 1;
    }

    OrbitControl(const Camera::sptr &camera) : OrbitControl(){
        this->camera = camera;
        //this->screen = screen;
        //this->object = object;

        target0.copy(target);
        position0.copy(camera->position);
        up0.copy(camera->up);
        zoom0 = camera->zoom;
    }

    float getAutoRotationAngle(){
        return 2 * math_number::PI / 60 / 60 * autoRotateSpeed;
    }

    float getZoomScale() {
        return math::pow( 0.95, zoomSpeed );
    }

    OrbitControl& rotateLeft( float angle ) {
        sphericalDelta.theta -= angle;
        return *this;
    }

    OrbitControl& rotateUp( float angle ) {
        sphericalDelta.phi -= angle;
        return *this;
    }

    OrbitControl& panLeft(float distance, Matrix4& objectMatrix);

    OrbitControl& panUp(float distance, Matrix4& objectMatrix);

    // deltaX and deltaY are in pixels; right and down are positive
    OrbitControl& pan( float deltaX,float deltaY );

    OrbitControl& dollyOut( float dollyScale );

    OrbitControl& dollyIn( float dollyScale );

    OrbitControl& handleMouseMoveDolly();

    OrbitControl& handleMouseMoveRotate();

    OrbitControl& handleMouseMovePan();

    bool update() override;

    //
    // event callbacks - update the object state
    //
    void reset() override;

    void keydown(threecpp::byte keyCode) override;

    void keyup() override;

    void mouseDown(unsigned button, float x, float y) override;

    void mouseMove(float x, float y) override;

    void mouseUp() override;

    void mouseWheel(float delta) override;

    void sizeChanged(const Vector4 &screen) override;

    Vector2 getMouseOnScreen(float pageX, float pageY);

    Vector3 getMouseProjectionOnBall(float pageX, float pageY);

//    OrbitControl& handleMouseDownRotate( event ) {
//
//        rotateStart.set( event.clientX, event.clientY );
//
//    }
//
//    function handleMouseDownDolly( event ) {
//
//        dollyStart.set( event.clientX, event.clientY );
//
//    }
//
//    function handleMouseDownPan( event ) {
//
//        panStart.set( event.clientX, event.clientY );
//
//    }
//
//    function handleMouseMoveRotate( event ) {
//
//        rotateEnd.set( event.clientX, event.clientY );
//
//        rotateDelta.subVectors( rotateEnd, rotateStart ).multiplyScalar( scope.rotateSpeed );
//
//        const element = scope.domElement;
//
//        rotateLeft( 2 * Math.PI * rotateDelta.x / element.clientHeight ); // yes, height
//
//        rotateUp( 2 * Math.PI * rotateDelta.y / element.clientHeight );
//
//        rotateStart.copy( rotateEnd );
//
//        scope.update();
//
//    }
//
//    function handleMouseMoveDolly( event ) {
//
//        dollyEnd.set( event.clientX, event.clientY );
//
//        dollyDelta.subVectors( dollyEnd, dollyStart );
//
//        if ( dollyDelta.y > 0 ) {
//
//            dollyOut( getZoomScale() );
//
//        } else if ( dollyDelta.y < 0 ) {
//
//            dollyIn( getZoomScale() );
//
//        }
//
//        dollyStart.copy( dollyEnd );
//
//        scope.update();
//
//    }
//
//    function handleMouseMovePan( event ) {
//
//        panEnd.set( event.clientX, event.clientY );
//
//        panDelta.subVectors( panEnd, panStart ).multiplyScalar( scope.panSpeed );
//
//        pan( panDelta.x, panDelta.y );
//
//        panStart.copy( panEnd );
//
//        scope.update();
//
//    }
//
//    function handleMouseWheel( event ) {
//
//        if ( event.deltaY < 0 ) {
//
//            dollyIn( getZoomScale() );
//
//        } else if ( event.deltaY > 0 ) {
//
//            dollyOut( getZoomScale() );
//
//        }
//
//        scope.update();
//
//    }
//
//    function handleKeyDown( event ) {
//
//        let needsUpdate = false;
//
//        switch ( event.code ) {
//
//            case scope.keys.UP:
//                pan( 0, scope.keyPanSpeed );
//                needsUpdate = true;
//                break;
//
//            case scope.keys.BOTTOM:
//                pan( 0, - scope.keyPanSpeed );
//                needsUpdate = true;
//                break;
//
//            case scope.keys.LEFT:
//                pan( scope.keyPanSpeed, 0 );
//                needsUpdate = true;
//                break;
//
//            case scope.keys.RIGHT:
//                pan( - scope.keyPanSpeed, 0 );
//                needsUpdate = true;
//                break;
//
//        }
//
//        if ( needsUpdate ) {
//
//            // prevent the browser from scrolling on cursor keys
//            event.preventDefault();
//
//            scope.update();
//
//        }
//
//
//    }
//
//    function handleTouchStartRotate() {
//
//        if ( pointers.length === 1 ) {
//
//            rotateStart.set( pointers[ 0 ].pageX, pointers[ 0 ].pageY );
//
//        } else {
//
//            const x = 0.5 * ( pointers[ 0 ].pageX + pointers[ 1 ].pageX );
//            const y = 0.5 * ( pointers[ 0 ].pageY + pointers[ 1 ].pageY );
//
//            rotateStart.set( x, y );
//
//        }
//
//    }
//
//    function handleTouchStartPan() {
//
//        if ( pointers.length === 1 ) {
//
//            panStart.set( pointers[ 0 ].pageX, pointers[ 0 ].pageY );
//
//        } else {
//
//            const x = 0.5 * ( pointers[ 0 ].pageX + pointers[ 1 ].pageX );
//            const y = 0.5 * ( pointers[ 0 ].pageY + pointers[ 1 ].pageY );
//
//            panStart.set( x, y );
//
//        }
//
//    }
//
//    function handleTouchStartDolly() {
//
//        const dx = pointers[ 0 ].pageX - pointers[ 1 ].pageX;
//        const dy = pointers[ 0 ].pageY - pointers[ 1 ].pageY;
//
//        const distance = Math.sqrt( dx * dx + dy * dy );
//
//        dollyStart.set( 0, distance );
//
//    }
//
//    function handleTouchStartDollyPan() {
//
//        if ( scope.enableZoom ) handleTouchStartDolly();
//
//        if ( scope.enablePan ) handleTouchStartPan();
//
//    }
//
//    function handleTouchStartDollyRotate() {
//
//        if ( scope.enableZoom ) handleTouchStartDolly();
//
//        if ( scope.enableRotate ) handleTouchStartRotate();
//
//    }
//
//    function handleTouchMoveRotate( event ) {
//
//        if ( pointers.length == 1 ) {
//
//            rotateEnd.set( event.pageX, event.pageY );
//
//        } else {
//
//            const position = getSecondPointerPosition( event );
//
//            const x = 0.5 * ( event.pageX + position.x );
//            const y = 0.5 * ( event.pageY + position.y );
//
//            rotateEnd.set( x, y );
//
//        }
//
//        rotateDelta.subVectors( rotateEnd, rotateStart ).multiplyScalar( scope.rotateSpeed );
//
//        const element = scope.domElement;
//
//        rotateLeft( 2 * Math.PI * rotateDelta.x / element.clientHeight ); // yes, height
//
//        rotateUp( 2 * Math.PI * rotateDelta.y / element.clientHeight );
//
//        rotateStart.copy( rotateEnd );
//
//    }
//
//    function handleTouchMovePan( event ) {
//
//        if ( pointers.length === 1 ) {
//
//            panEnd.set( event.pageX, event.pageY );
//
//        } else {
//
//            const position = getSecondPointerPosition( event );
//
//            const x = 0.5 * ( event.pageX + position.x );
//            const y = 0.5 * ( event.pageY + position.y );
//
//            panEnd.set( x, y );
//
//        }
//
//        panDelta.subVectors( panEnd, panStart ).multiplyScalar( scope.panSpeed );
//
//        pan( panDelta.x, panDelta.y );
//
//        panStart.copy( panEnd );
//
//    }
//
//    function handleTouchMoveDolly( event ) {
//
//        const position = getSecondPointerPosition( event );
//
//        const dx = event.pageX - position.x;
//        const dy = event.pageY - position.y;
//
//        const distance = Math.sqrt( dx * dx + dy * dy );
//
//        dollyEnd.set( 0, distance );
//
//        dollyDelta.set( 0, Math.pow( dollyEnd.y / dollyStart.y, scope.zoomSpeed ) );
//
//        dollyOut( dollyDelta.y );
//
//        dollyStart.copy( dollyEnd );
//
//    }
//
//    function handleTouchMoveDollyPan( event ) {
//
//        if ( scope.enableZoom ) handleTouchMoveDolly( event );
//
//        if ( scope.enablePan ) handleTouchMovePan( event );
//
//    }
//
//    function handleTouchMoveDollyRotate( event ) {
//
//        if ( scope.enableZoom ) handleTouchMoveDolly( event );
//
//        if ( scope.enableRotate ) handleTouchMoveRotate( event );
//
//    }


};


#endif //THREE_CPP_ORBIT_CONTROL_H
