#include "InputHandler.h"
#include <iostream>

bool InputHandler::useFrontCamera = false;
glm::vec3 InputHandler::frontCameraPos;
float InputHandler::lastX = 300, InputHandler::lastY = 300;
float InputHandler::yaw = -90.0f, InputHandler::pitch = 0.0f;
bool InputHandler::firstMouse = true;
float InputHandler::speed = 0.0f;
float InputHandler::car_pos_x = 0.0f, InputHandler::car_pos_y = 0.0f, InputHandler::car_pos_z = 0.0f;
float InputHandler::theta_mod_x = 0.0f, InputHandler::theta_mod_y = 0.0f, InputHandler::theta_mod_z = 0.0f;

int InputHandler::currentSkybox = 0; // Initialize the current skybox

void InputHandler::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (!useFrontCamera)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
}

void InputHandler::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_REPEAT)
	{
		if (speed > 1.00f)
		{
			speed = 1.0f;
		}
		if (key == GLFW_KEY_W)
		{
			speed += 0.01f;
			car_pos_z -= speed;
			if (theta_mod_y > 0)
				theta_mod_y -= 0.5f;
			else if (theta_mod_y < 0)
				theta_mod_y += 0.5f;
		}
		if (key == GLFW_KEY_S)
		{
			speed += 0.01f;
			car_pos_z += speed;
			if (theta_mod_y > 0)
				theta_mod_y -= 0.5f;
			else if (theta_mod_y < 0)
				theta_mod_y += 0.5f;
		}
		if (key == GLFW_KEY_A)
		{
			speed += 0.01f;
			car_pos_x -= 0.1f;
			car_pos_z -= speed;
			if (theta_mod_y < 10.f)
				theta_mod_y += 1.f;
		}
		if (key == GLFW_KEY_D)
		{
			speed += 0.01f;
			car_pos_x += 0.1f;
			car_pos_z -= speed;
			if (theta_mod_y > -10.f)
				theta_mod_y -= 1.f;
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			if (speed > 0.0f)
			{
				speed -= 0.01f;
				std::cout << speed << "\n";
				car_pos_z -= speed;
			}
		}
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			if (speed > 0.0f)
			{
				speed -= 0.01f;
				std::cout << speed << "\n";
				car_pos_z += speed;
			}
		}
		if (key == GLFW_KEY_Z && action == GLFW_PRESS)
		{
			useFrontCamera = !useFrontCamera;
		}
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			useFrontCamera = !useFrontCamera;
		}
		if (action == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_Q:
				currentSkybox = 0; // Morning skybox
				break;
			case GLFW_KEY_E:
				currentSkybox = 1; // Night skybox
				break;
			}
		}
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}
