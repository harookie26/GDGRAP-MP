#include "Camera.h"

Camera::Camera(const glm::vec3& position, const glm::vec3& up, float yaw, float pitch)
	: position(position), worldUp(up), yaw(yaw), pitch(pitch)
{
	updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
	return lookAt(position, position + front, up);
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= 0.1f;
	yoffset *= 0.1f;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateVectors();
}

void Camera::updateVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	this->front = normalize(front);
	right = normalize(cross(this->front, worldUp));
	up = normalize(cross(right, this->front));
}
