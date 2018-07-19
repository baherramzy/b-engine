#ifndef CAMERA
#define CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    private:
        glm::vec3 pos, front, up;
        float pitch, yaw;
        float minPitch, maxPitch;

    public:
        unsigned int cameraID;
        float mouseSensitivity;

        /* Constructor */
        Camera();

        /* Getters */
        const float& getPitch() const;
        const float& getYaw() const;

        const glm::vec3 getRight() const;
        const glm::mat4 getViewMatrix() const;

        /* Setters */
        void setPos(const glm::vec3 &newPos);
        void setFront(const glm::vec3 &newFront);
        void setUp(const glm::vec3 &newUp);

        void setPitch(const float &newPitch);
        void setYaw(const float &newYaw);

        /* Movement methods */
        void moveForward(const float &speed);
        void moveBackward(const float &speed);
        void moveLeft(const float &speed);
        void moveRight(const float &speed);

        /* Mouse event handler */
        void processMouseMovement(const float xOffset, const float yOffset);

        /* Utility methods */
        void addToPos(const glm::vec3 &posDelta);
        void addPitch(const float &pitchOffset);
        void addYaw(const float &yawOffset);
};

#endif