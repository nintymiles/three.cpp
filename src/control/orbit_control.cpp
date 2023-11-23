//
// Created by ultraman on 11/22/2023.
//

#include "orbit_control.h"

#include "perspective_camera.h"
#include "orthographic_camera.h"
#include "quaternion.h"

OrbitControl& OrbitControl::panLeft(float distance, Matrix4& objectMatrix) {
    Vector3 v{};

    v.setFromMatrixColumn( objectMatrix, 0 ); // get X column of objectMatrix
    v.multiplyScalar( - distance );

    panOffset.add( v );
    return *this;
};

OrbitControl& OrbitControl::panUp(float distance, Matrix4& objectMatrix) {
    Vector3 v{};
    if ( screenSpacePanning == true ) {
        v.setFromMatrixColumn( objectMatrix, 1 );
    } else {
        v.setFromMatrixColumn( objectMatrix, 0 );
        v.crossVectors( object->up, v );
    }

    v.multiplyScalar( distance );
    panOffset.add( v );

    return * this;
};

// deltaX and deltaY are in pixels; right and down are positive
OrbitControl& OrbitControl::pan( float deltaX,float deltaY ) {
    Vector3 offset{};

    //if(instanceOf<PerspectiveCamera>(object.get())) {
    if(std::dynamic_pointer_cast<PerspectiveCamera>(object)){
        PerspectiveCamera::sptr pCamera = std::dynamic_pointer_cast<PerspectiveCamera>(object);
        // perspective
        Vector3 position = pCamera->position;
        offset.copy( position ).sub( target );
        float targetDistance = offset.length();

        // half of the fov is center to top of screen
        targetDistance *= math::tan( ( pCamera->fov / 2 ) * math_number::PI / 180.0 );

        // we use only clientHeight here so aspect ratio does not distort speed
//            panLeft( 2 * deltaX * targetDistance / element.clientHeight, scope.object.matrix );
//            panUp( 2 * deltaY * targetDistance / element.clientHeight, scope.object.matrix );

    } else if (std::dynamic_pointer_cast<OrthographicCamera>(object)) { // orthographic
        OrthographicCamera::sptr oCamera = std::dynamic_pointer_cast<OrthographicCamera>(object);

//            panLeft( deltaX * ( oCamera->right - oCamera->left ) / oCamera->zoom / element.clientWidth, oCamera->matrix );
//            panUp( deltaY * ( oCamera->top - oCamera->bottom ) / oCamera->zoom / element.clientHeight, oCamera->matrix );

    } else {
        // camera neither orthographic nor perspective
        std::cerr << ( "WARNING: OrbitControls encountered an unknown camera type - pan disabled." ) << std::endl;
        enablePan = false;
    }
    return *this;
};



OrbitControl& OrbitControl::dollyOut( float dollyScale ) {
    if (std::dynamic_pointer_cast<PerspectiveCamera>(this->object)) {
        scale /= dollyScale;
    }else if(std::dynamic_pointer_cast<OrthographicCamera>(this->object)) {
        OrthographicCamera::sptr oCamera = std::dynamic_pointer_cast<OrthographicCamera>(this->object);
        oCamera->zoom = fmax( minZoom, fmin( maxZoom, oCamera->zoom * dollyScale ) );
        oCamera->updateProjectionMatrix();
        zoomChanged = true;
    } else {
        std::cerr << ( "WARNING: OrbitControls encountered an unknown camera type - dolly/zoom disabled." )<< std::endl;
        enableZoom = false;
    }
    return *this;
}

OrbitControl& OrbitControl::dollyIn( float dollyScale ) {
    if (std::dynamic_pointer_cast<PerspectiveCamera>(this->object)) {
        scale *= dollyScale;
    } else if (std::dynamic_pointer_cast<OrthographicCamera>(this->object)) {
        OrthographicCamera::sptr oCamera = std::dynamic_pointer_cast<OrthographicCamera>(this->object);
        oCamera->zoom = fmax( minZoom, fmin( maxZoom, oCamera->zoom / dollyScale ) );
        oCamera->updateProjectionMatrix();
        zoomChanged = true;

    } else {
        std::cerr << ( "WARNING: OrbitControls encountered an unknown camera type - dolly/zoom disabled." )<< std::endl;
        enableZoom = false;
    }
    return *this;
}

OrbitControl& OrbitControl::handleMouseMoveRotate() {
    //rotateEnd.set( event.clientX, event.clientY );
    rotateDelta.subVectors( rotateEnd, rotateStart ).multiplyScalar( rotateSpeed );
    //const element = scope.domElement;
    rotateLeft( 2 * math_number::PI * rotateDelta.x / screen.w ); // yes, height

    rotateUp( 2 * math_number::PI * rotateDelta.y / screen.w );
    rotateStart.copy( rotateEnd );
    update();
    return *this;
}

OrbitControl& OrbitControl::handleMouseMoveDolly() {
    //dollyEnd.set( event.clientX, event.clientY );
    dollyDelta.subVectors( dollyEnd, dollyStart );

    if ( dollyDelta.y > 0 ) {
        dollyOut( getZoomScale() );
    } else if ( dollyDelta.y < 0 ) {
        dollyIn( getZoomScale() );
    }

    dollyStart.copy( dollyEnd );
    update();

    return *this;
}

OrbitControl& OrbitControl::handleMouseMovePan() {
    //panEnd.set( event.clientX, event.clientY );
    panDelta.subVectors( panEnd, panStart ).multiplyScalar( panSpeed );
    pan( panDelta.x, panDelta.y );
    panStart.copy( panEnd );
    update();
    return *this;
}



void OrbitControl::reset(){
    state = STATE::NONE;
//    keyState = STATE::NONE;

    target.copy(target0);
    camera->position.copy(position0);
    camera->up.copy(up0);
    camera->zoom = zoom0;

    camera->updateProjectionMatrix();

//    eye.subVectors(camera->position, target);

    camera->lookAt(target);

    //_this.dispatchEvent(changeEvent);
    lastPosition.copy(camera->position);
    lastZoom = camera->zoom;
}

void OrbitControl::sizeChanged(const Vector4& screen){
    //Vector4
    //X->left, Y->top,Z->width, W->height
    this->screen.copy(screen);
    camera->aspect = (float)(screen.z / screen.w);
    camera->updateProjectionMatrix();
}

void OrbitControl::keydown(byte keyCode){
    if (enabled == false) return;

    if (keyState != STATE::NONE) {
        return;
    }else if(keyCode == keys[(byte)STATE::ROTATE] && !noRotate) {
        keyState = STATE::ROTATE;
    }else if(keyCode == keys[(byte)STATE::DOLLY] && !noZoom) {
        keyState = STATE::DOLLY;
    }else if(keyCode == keys[(byte)STATE::PAN] && !noPan) {
        keyState = STATE::PAN;
    }
}

void OrbitControl::keyup(){
    if (enabled == false) return;

    keyState = STATE::NONE;
}

void OrbitControl::mouseDown(unsigned button, float x, float y){
    if (!enabled) return;

    if (state == STATE::NONE){
        switch (button){
            case 0: //MouseButtons.Left
                state = STATE::ROTATE;
                mouseAction = MouseButton::LEFT;
                break;
            case 1://MouseButtons.Middle:
                state = STATE::DOLLY;
                mouseAction = MouseButton::MIDDLE;
                break;
            case 2: //MouseButtons.Right:
                state = STATE::PAN;
                mouseAction = MouseButton::RIGHT;
                break;
        }
    }

    switch (state) {
        case STATE::ROTATE:
            if ( enableRotate == false ) return;
            rotateStart = getMouseOnScreen(x, y);
            rotateEnd = rotateStart;
            break;
        case STATE::DOLLY:
            if ( enableZoom == false ) return;
            dollyStart = getMouseOnScreen(x, y);
            dollyEnd = dollyStart;
            break;
        case STATE::PAN:
            if ( enablePan == false ) return;
            panStart = getMouseOnScreen(x, y);
            panEnd = panStart;
            break;

    }

//    if (state == STATE::ROTATE && !noRotate){
//        rotateStart = getMouseProjectionOnBall(x, y);
//        rotateEnd = rotateStart;
//
//    }else if(state == STATE::ZOOM && !noZoom){
//        zoomStart = getMouseOnScreen(x, y);
//        zoomEnd = zoomStart;
//
//    }else if (state == STATE::PAN && !noPan){
//        panStart = getMouseOnScreen(x, y);
//        panEnd = panStart;
//
//    }
}

// if sender was unfocused, then you must focus that
// if(sender.focused==false) sender.focus()
void OrbitControl::mouseMove(float x, float y){
    //Vector2 mPos = getMouseOnScreen(x,y);
    //mousePos.set(x - screen.x,y - screen.y);
    if (!enabled) return;

    if (state == STATE::ROTATE && !noRotate){
        rotateEnd = getMouseOnScreen(x, y);
        handleMouseMoveRotate();
    }else if(state == STATE::DOLLY && !noZoom){
        dollyEnd = getMouseOnScreen(x, y);
        handleMouseMoveDolly();
    }else if(state == STATE::PAN && !noPan){
        panEnd = getMouseOnScreen(x, y);
        handleMouseMovePan();
    }
}

void OrbitControl::mouseUp(){
    if (!enabled) return;
    state = STATE::NONE;
}

void OrbitControl::mouseWheel(float delta){
    if (!enabled) return;

    float _delta = delta / 40;

    dollyStart.y += _delta * 0.01f;
}

Vector2 OrbitControl::getMouseOnScreen(float pageX, float pageY){
    return Vector2(
            (pageX - screen.x) / screen.z,
            (pageY - screen.y) / screen.w);

}

Vector3 OrbitControl::getMouseProjectionOnBall(float pageX, float pageY){
    Vector3 mouseOnBall = Vector3(
            (pageX - screen.z * 0.5f - screen.x) / (screen.z * 0.5f),
            (screen.w * 0.5f + screen.y - pageY) / (screen.w * 0.5f),
            0.0f
    );

    float length = mouseOnBall.length();

    if (noRoll){
        if (length < math_number::SQRT1_2)
            mouseOnBall.z = (float)sqrt(1.0f - length * length);
        else
            mouseOnBall.z = 0.5f / length;
    }else if (length > 1.0f)
        mouseOnBall.normalize();
    else
        mouseOnBall.z = (float)sqrt(1.0f - length * length);

    Vector3 camPos = camera->position;
    eye = camPos - target;
    Vector3 upClone;
    upClone.copy(camera->up);

    Vector3 projection;
    upClone.normalize();

    projection = upClone.multiplyScalar(mouseOnBall.y);

    Vector3 cross;
    cross.copy(camera->up);
    cross.cross(eye);
    cross.normalize();
    cross.multiplyScalar(mouseOnBall.x);

    projection = projection.add(cross);

    //  projection.add(_eye.normalize().scale(mouseOnBall.z));
    Vector3 eyeClone;
    eyeClone.copy(eye);
    eyeClone.normalize();

    projection.add(eyeClone.multiplyScalar(mouseOnBall.z));

    return projection;
}

bool OrbitControl::update() {
    Vector3 offset{}; // so camera.up is the orbit axis

    Quaternion quat = Quaternion().setFromUnitVectors( object->up, Vector3( 0, 1, 0 ) );
    Quaternion quatInverse = quat.clone().inverse();

    float twoPI = math_number::PI2;

    Vector3& position = object->position;
    offset.copy( position ).sub( target ); // rotate offset to "y-axis-is-up" space

    offset.applyQuaternion( quat ); // angle from z-axis around y-axis

    spherical.setFromVector3( offset );

    if ( autoRotate && state == STATE::NONE ) {
        rotateLeft( getAutoRotationAngle() );
    }

    if ( enableDamping ) {
        spherical.theta += sphericalDelta.theta * dampingFactor;
        spherical.phi += sphericalDelta.phi * dampingFactor;
    } else {
        spherical.theta += sphericalDelta.theta;
        spherical.phi += sphericalDelta.phi;
    } // restrict theta to be between desired limits


    float min = minAzimuthAngle;
    float max = maxAzimuthAngle;

    if ( std::isfinite( min ) && std::isfinite( max ) ) {

        if ( min < - math_number::PI ) min += twoPI; else if ( min > math_number::PI ) min -= twoPI;
        if ( max < - math_number::PI ) max += twoPI; else if ( max > math_number::PI ) max -= twoPI;

        if ( min <= max ) {
            spherical.theta = fmax( min, fmin( max, spherical.theta ) );
        } else {
            spherical.theta = spherical.theta > ( min + max ) / 2 ? fmax( min, spherical.theta ) : fmin( max, spherical.theta );
        }

    } // restrict phi to be between desired limits


    spherical.phi = fmax( minPolarAngle, fmin( maxPolarAngle, spherical.phi ) );
    spherical.makeSafe();
    spherical.radius *= scale; // restrict radius to be between desired limits

    spherical.radius = fmax( minDistance, fmin( maxDistance, spherical.radius ) ); // move target to panned location

    if ( enableDamping == true ) {
        target.addScaledVector( panOffset, dampingFactor );
    } else {
        target.add( panOffset );
    }

    offset.setFromSpherical( spherical ); // rotate offset back to "camera-up-vector-is-up" space

    offset.applyQuaternion( quatInverse );
    position.copy( target ).add( offset );
    object->lookAt( target );

    if ( enableDamping == true ) {
        sphericalDelta.theta *= 1 - dampingFactor;
        sphericalDelta.phi *= 1 - dampingFactor;
        panOffset.multiplyScalar( 1 - dampingFactor );
    } else {
        sphericalDelta.set( 0, 0, 0 );
        panOffset.set( 0, 0, 0 );
    }

    scale = 1; // update condition is:
    // min(camera displacement, camera rotation in radians)^2 > EPS
    // using small-angle approximation cos(x/2) = 1 - x^2 / 8

    if ( zoomChanged || lastPosition.distanceToSquared( object->position ) > EPS || 8 * ( 1 - lastQuaternion.dot( object->quaternion ) ) > EPS ) {
        //todo:fix this event handle
        //scope.dispatchEvent( _changeEvent );
        lastPosition.copy( object->position );
        lastQuaternion.copy( object->quaternion );
        zoomChanged = false;
        return true;
    }

    return false;
}
