#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch);
    glm::mat4 getViewMatrix() const;
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void updateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

private:
    void updateVectors();
};

#endif // CAMERA_H
