#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputHandler
{
public:
    static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

    static bool useFrontCamera;
    static glm::vec3 frontCameraPos;
    static float lastX, lastY;
    static float yaw, pitch;
    static bool firstMouse;
    static float speed;
    static float car_pos_x, car_pos_y, car_pos_z;
    static float theta_mod_x, theta_mod_y, theta_mod_z;
};

#endif // INPUTHANDLER_H
