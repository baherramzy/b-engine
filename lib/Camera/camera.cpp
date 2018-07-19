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
    , mouseSensitivity { 0.07f }
{}

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

/* Mouse event handler */
/* Updates the camera's orientation based on mouse movement */
void Camera::processMouseMovement(const float xOffset, const float yOffset)
{
    // Update yaw and pitch values based on horizontal
    // and vertical mouse movements, respectively
    addYaw(xOffset * mouseSensitivity);
    addPitch(yOffset * mouseSensitivity);

    // Compute new front vector
    glm::vec3 mouseFront;
    mouseFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    mouseFront.y = sin(glm::radians(pitch));
    mouseFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    front = glm::normalize(mouseFront);
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