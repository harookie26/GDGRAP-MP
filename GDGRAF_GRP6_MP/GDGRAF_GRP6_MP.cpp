#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.h"
#include "Renderer.h"
#include "Skybox.h"
#include "Lighting.h"
#include "ObjectLoader.h"
#include "TextureLoader.h"
#include "InputHandler.h"
#include "Camera.h"

float lightX = -10;
float lightY = 3;
float lightZ = 0;

std::string morningSkybox[]{
    "Skybox/morningSkybox/mRt.png",
    "Skybox/morningSkybox/mLf.png",
    "Skybox/morningSkybox/mUp.png",
    "Skybox/morningSkybox/mDn.png",
    "Skybox/morningSkybox/mFt.png",
    "Skybox/morningSkybox/mBk.png"
};

std::string nightSkybox[]{
    "Skybox/nightSkybox/mRt.png",
    "Skybox/nightSkybox/mLf.png",
    "Skybox/nightSkybox/mUp.png",
    "Skybox/nightSkybox/mDn.png",
    "Skybox/nightSkybox/mFt.png",
    "Skybox/nightSkybox/mBk.png"
};

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    float windowWidth = 1000;
    float windowHeight = 700;

    window = glfwCreateWindow(windowWidth, windowHeight, "Ram tempo", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGL();

    glfwSetKeyCallback(window, InputHandler::keyCallback);
    glfwSetCursorPosCallback(window, InputHandler::mouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    Shader shaderProg("Shaders/Sam.vert", "Shaders/Sam.frag");
    Shader skyboxShaderProg("Shaders/Skybox.vert", "Shaders/Skybox.frag");

    std::vector<std::string> facesMorningSkybox(morningSkybox, morningSkybox + sizeof(morningSkybox) / sizeof(std::string));
    std::vector<std::string> facesNightSkybox(nightSkybox, nightSkybox + sizeof(nightSkybox) / sizeof(std::string));

    Skybox morningSkyboxObj(facesMorningSkybox);
    Skybox nightSkyboxObj(facesNightSkybox);

    Skybox* currentSkybox = &morningSkyboxObj;

    Renderer renderer(windowWidth, windowHeight);

    // Set the light color to bright yellow, increase intensity, and set direction
    Lighting lighting(glm::vec3(lightX, lightY, lightZ), glm::vec3(1.0f, 1.0f, 1.0f), 0.1f, 10.0f, 32.0f, Lighting::LightType::DIRECTIONAL, glm::vec3(-0.5f, -1.0f, -0.5f));
    renderer.setLight(lighting);

    ObjectLoader objectLoader;
    std::vector<GLfloat> fullVertexData;
    std::vector<GLuint> mesh_indices;
    objectLoader.loadObject("3D/Car.obj", fullVertexData, mesh_indices);

    GLuint texture = TextureLoader::loadTexture("3D/brickwall.jpg");
    GLuint norm_tex = TextureLoader::loadTexture("3D/brickwall_normal.jpg");

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normalPtr);
    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(2);

    GLintptr tangentPtr = 8 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)tangentPtr);
    glEnableVertexAttribArray(3);

    GLintptr bitangentPtr = 11 * sizeof(float);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)bitangentPtr);
    glEnableVertexAttribArray(4);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    auto identity_matrix = glm::mat4(1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lighting.setPosition(glm::vec3(lightX, lightY, lightZ));
        renderer.setLight(lighting);

        shaderProg.use();
        shaderProg.setInt("lightType", 1); // 1 for directional light
        shaderProg.setVec3("lightDir", glm::vec3(-0.5f, -1.0f, -0.5f));
        shaderProg.setVec3("lightColor", lighting.getColor());

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), windowWidth / windowHeight, 0.1f, 100.0f);

        glm::vec3 cameraPos;
        if (InputHandler::useFrontCamera)
        {
            InputHandler::frontCameraPos = glm::vec3(InputHandler::car_pos_x, InputHandler::car_pos_y + 0.2f,
                InputHandler::car_pos_z - 0.3f);
            cameraPos = InputHandler::frontCameraPos;
        }
        else
        {
            cameraPos = glm::vec3(InputHandler::car_pos_x, InputHandler::car_pos_y + 0.5f,
                InputHandler::car_pos_z + 1.5f);
        }

        glm::vec3 front;
        front.x = cos(glm::radians(InputHandler::yaw)) * cos(glm::radians(InputHandler::pitch));
        front.y = sin(glm::radians(InputHandler::pitch));
        front.z = sin(glm::radians(InputHandler::yaw)) * cos(glm::radians(InputHandler::pitch));
        front = normalize(front);

        glm::vec3 cameraCenter = cameraPos + front;
        glm::vec3 worldUp = normalize(glm::vec3(0.0f, 1.0f, 0.0f));

        renderer.setCamera(cameraPos, front, worldUp);
        // Update current skybox based on currentSkybox
        if (InputHandler::currentSkybox == 0)
        {
            currentSkybox = &morningSkyboxObj;
        }
        else if (InputHandler::currentSkybox == 1)
        {
            currentSkybox = &nightSkyboxObj;
        }

        currentSkybox->render(skyboxShaderProg, renderer.getViewMatrix(), projectionMatrix);

        renderer.renderModel(shaderProg, VAO, fullVertexData,
            glm::vec3(InputHandler::car_pos_x, InputHandler::car_pos_y, InputHandler::car_pos_z),
            glm::vec3(0.1f),
            glm::vec3(InputHandler::theta_mod_x, InputHandler::theta_mod_y, InputHandler::theta_mod_z),
            1.0f);
        renderer.renderModel(shaderProg, VAO, fullVertexData, glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.1f),
            glm::vec3(InputHandler::theta_mod_x, InputHandler::theta_mod_y, InputHandler::theta_mod_z),
            0.5f);
        renderer.renderModel(shaderProg, VAO, fullVertexData, glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.1f),
            glm::vec3(InputHandler::theta_mod_x, InputHandler::theta_mod_y, InputHandler::theta_mod_z),
            0.5f);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}
