#include "camera.h"

/* Constructor */
Camera::Camera()
    : pos { glm::vec3(0.0f, 0.0f, 6.0f) }
    , front { glm::vec3(0.0f, 0.0f, -1.0f) }
    , up { glm::vec3(0.0f, 1.0f, 0.0f) }
    , pitch { 0.0f }
    , yaw { -90.0f }
    , minPitch { -89.0f }
    , maxPitch { 89.0f }
{}

/* Getters */
const float& Camera::getPitch() const
{
    return pitch;
}

const float& Camera::getYaw() const
{
    return yaw;
}

const glm::vec3 Camera::getRight() const
{
    return glm::cross(front, up);
}

const glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(pos, pos + front, up);
}

void Camera::moveForward(const float &speed)
{
    Camera::addToPos(speed * front);
}

void Camera::moveBackward(const float &speed)
{
    Camera::addToPos(-speed * front);    
}

void Camera::moveLeft(const float &speed)
{
    const glm::vec3 right = Camera::getRight();
    Camera::addToPos(glm::normalize(right) * -speed);
}

void Camera::moveRight(const float &speed)
{
    const glm::vec3 right = Camera::getRight();
    Camera::addToPos(glm::normalize(right) * speed);
}

/* Setters */
void Camera::setPos(const glm::vec3 &newPos)
{
    pos = newPos;
}

void Camera::setFront(const glm::vec3 &newFront)
{
    front = newFront;
}

void Camera::setUp(const glm::vec3 &newUp)
{
    up = newUp;
}

void Camera::setPitch(const float &newPitch)
{
    pitch = newPitch;
}

void Camera::setYaw(const float &newYaw)
{
    yaw = newYaw;
}

/* Utility methods */
void Camera::addToPos(const glm::vec3 &posDelta)
{
    pos = pos + posDelta;
}

void Camera::addPitch(const float &pitchOffset)
{
    /* Limit vertical look angles to prevent odd view rotations */
    if  ((pitch + pitchOffset) > maxPitch)      pitch = maxPitch;
    else if ((pitch + pitchOffset) < minPitch)  pitch = minPitch;
    else pitch = pitch + pitchOffset;
}

void Camera::addYaw(const float &yawOffset)
{
    yaw = yaw + yawOffset;
}