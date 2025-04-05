#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Shader.h"
#include "Renderer.h"
#include "Skybox.h"
#include "Lighting.h"

float brightness = 5.0f;

float camera_mod_x = 0.0f;
float camera_mod_y = 0.0f;
float camera_mod_z = -1.f;

float x_mod = 0.0f;
float y_mod = 0.0f;
float z_mod = 3.0f;

float speed = 0.0f;

float scale_x = 1;
float scale_y = 1;
float scale_z = 1;

float lightX = -10;
float lightY = 3;
float lightZ = 0;

float theta_mod_x = 0.0f;
float theta_mod_y = 0.0f;
float theta_mod_z = 0.0f;

float axis_x = 0.0f;
float axis_y = 1.0f;
float axis_z = 0.0f;

float car_pos_x = 0.0f;
float car_pos_y = 0.0f;
float car_pos_z = 0.0f;

float lastX = 300, lastY = 300; // Initial mouse position
float yaw = -90.0f, pitch = 0.0f; // Initial orientation
bool firstMouse = true;

// Add these global variables
bool useFrontCamera = false;
glm::vec3 frontCameraPos;

//wah
//R, L, U, D, F, B
std::string facesSkybox[]{
    "Skybox/rainbow_rt.png",
    "Skybox/rainbow_lf.png",
    "Skybox/rainbow_up.png",
    "Skybox/rainbow_dn.png",
    "Skybox/rainbow_ft.png",
    "Skybox/rainbow_bk.png"
};

//R, L, U, D, F, B
std::string MornSkybox[]{
    "Skybox/rainbow_rt.png",
    "Skybox/rainbow_lf.png",
    "Skybox/rainbow_up.png",
    "Skybox/rainbow_dn.png",
    "Skybox/rainbow_ft.png",
    "Skybox/rainbow_bk.png"
};

void Mouse_Callback(GLFWwindow* window, double xpos, double ypos)
{
    if (!useFrontCamera) // Only update orientation if rear camera is active
    {
        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to top
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

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
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

        if (key==GLFW_KEY_A)  // (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
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
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
			if (speed > 0.0f)
			{
				speed -= 0.01f;

                std::cout << speed<<"\n";
                
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
    }
   
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE); // Closes the window by pressing the Escape key
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float windowWidth = 1000;
    float windowHeight = 700;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(windowWidth, windowHeight, "Ram tempo", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    // Initialize Glad
    gladLoadGL();

    glfwSetKeyCallback(window, Key_Callback);
    glfwSetCursorPosCallback(window, Mouse_Callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLfloat UV[] = {
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    stbi_set_flip_vertically_on_load(true);

    int img_width, img_height, color_channels; // color channels range from 3 - 4 (RGB - RGBA)
    // 3 == RGB JPGS !transparency
    // 4 == RGBA PNGS transparency

    // texture
    unsigned char* tex_bytes = stbi_load(
        "3D/brickwall.jpg",
        &img_width,
        &img_height,
        &color_channels,
        0
    );

    Shader shaderProg("Shaders/Sam.vert", "Shaders/Sam.frag");
    Shader skyboxShaderProg("Shaders/Skybox.vert", "Shaders/Skybox.frag");

    // Convert array to vector
    std::vector<std::string> facesSkyboxVec(facesSkybox, facesSkybox + sizeof(facesSkybox) / sizeof(std::string));

    // Create Skybox instance
    Skybox skybox(facesSkyboxVec);

    Renderer renderer(windowWidth, windowHeight);

    Lighting lighting(glm::vec3(lightX, lightY, lightZ), glm::vec3(1, 1, 1), 0.1f, 5.0f, 16.0f);
    renderer.setLight(lighting);

    std::string path = "3D/Car.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector < tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes; //positions, texture data, and etc.

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()
    );

    std::vector<GLuint> mesh_indices;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[i].vertex_index);
    }

    std::vector<glm::vec3>tangents;
    std::vector<glm::vec3>bitangents;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i += 3)
    {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        //position points

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[(vData1.vertex_index * 3)],
            attributes.vertices[(vData1.vertex_index * 3) + 1],
            attributes.vertices[(vData1.vertex_index * 3) + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[(vData2.vertex_index * 3)],
            attributes.vertices[(vData2.vertex_index * 3) + 1],
            attributes.vertices[(vData2.vertex_index * 3) + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[(vData3.vertex_index * 3)],
            attributes.vertices[(vData3.vertex_index * 3) + 1],
            attributes.vertices[(vData3.vertex_index * 3) + 2]
        );

        //UV points

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[(vData1.texcoord_index * 2)],
            attributes.texcoords[(vData1.texcoord_index * 2) + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[(vData2.texcoord_index * 2)],
            attributes.texcoords[(vData2.texcoord_index * 2) + 1]
        );

        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[(vData3.texcoord_index * 2)],
            attributes.texcoords[(vData3.texcoord_index * 2) + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;
        //
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / ((deltaUV1.x * deltaUV2.y) - (deltaUV1.y - deltaUV2.x));

        //tangent and bitangent time

        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV1.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);

    }

    std::vector<GLfloat> fullVertexData;

    for (int i = 0; i < shapes[0].mesh.indices.size(); i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        //(XYZ coordinates)
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        //(Normal XYZ)
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
        fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);

        //(UV)
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
        fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);

        ///tangetn bitangent

        fullVertexData.push_back(tangents[i].x);
        fullVertexData.push_back(tangents[i].y);
        fullVertexData.push_back(tangents[i].z);

        fullVertexData.push_back(bitangents[i].x);
        fullVertexData.push_back(bitangents[i].y);
        fullVertexData.push_back(bitangents[i].z);
    }
    GLfloat vertices[]
    {
        0.f, 0.5f, 0.f,
        -0.5f, 0.0f, 0.f,
        0.5f, 0.f, 0.f
    };

    GLuint indices[]
    {
        0, 1, 2
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB, //RGBA if 4 channels and RGB if 3 channels
        img_width,
        img_height,
        0, //border but if 0 it means no border
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);
    //reuse tex_bytes
    tex_bytes = stbi_load(
        "3D/brickwall_normal.jpg",
        &img_width,
        &img_height,
        &color_channels,
        0
    );

    //normal map texture
    GLuint norm_tex;
    glGenTextures(1, &norm_tex);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, norm_tex);

    //tile in the x axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    //tile in the y axis
    glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        img_width,
        img_height,
        0,
        GL_RGB,
        GL_UNSIGNED_BYTE,
        tex_bytes
    );

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(tex_bytes);

    //ID of VAO & VBO & EBO
    GLuint VAO, VBO, EBO, VBO_UV;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //current VAO = null
    glBindVertexArray(VAO);
    //current VAO = VAO

    //Current VBO = null
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Current VBO = VBO
    //current VAO.VBO.append(VBO)

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0); //whole vector

    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)normalPtr); //offset point for normal so start at index 3

    GLintptr uvPtr = 6 * sizeof(float);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)uvPtr); //offset point for UV so start at index 6

    GLintptr tangentPtr = 8 * sizeof(float);
    GLintptr bitangentPtr = 11 * sizeof(float);

    glVertexAttribPointer(
        3,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)tangentPtr
    );

    glVertexAttribPointer(
        4,
        3,
        GL_FLOAT,
        GL_FALSE,
        14 * sizeof(float),
        (void*)bitangentPtr
    );

    //enables attrib index 0
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    glm::mat4 identity_matrix = glm::mat4(1.0f);

    //glm::mat4 projectionMatrix = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);

    ///ALPHA BLEND
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);//destubatuib
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lighting.setPosition(glm::vec3(lightX, lightY, lightZ));
        renderer.setLight(lighting);

        glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        // (FOV, Aspect Ratio, zNear, zFar)

        // Position Matrix of Camera
        glm::vec3 cameraPos;
        if (useFrontCamera)
        {
            frontCameraPos = glm::vec3(car_pos_x, car_pos_y + 0.2f, car_pos_z - 0.3f); // Move camera in front of the car
            cameraPos = frontCameraPos;
        }
        else
        {
            cameraPos = glm::vec3(car_pos_x, car_pos_y + 0.5f, car_pos_z + 1.5f); // Move camera above and behind the car
        }

        // Calculate the new orientation
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(front);

        glm::vec3 cameraCenter = cameraPos + front;
        glm::vec3 worldUp = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)); // Pointing upwards

        renderer.setCamera(cameraPos, front, worldUp);

        // Render skybox
        skybox.render(skyboxShaderProg, renderer.getViewMatrix(), projectionMatrix);

        // Render main model (car)
        renderer.renderModel(shaderProg, VAO, fullVertexData, glm::vec3(car_pos_x, car_pos_y, car_pos_z), glm::vec3(0.1f), glm::vec3(theta_mod_x, theta_mod_y, theta_mod_z), 1.0f);

        // Render left model
        renderer.renderModel(shaderProg, VAO, fullVertexData, glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.1f), glm::vec3(theta_mod_x, theta_mod_y, theta_mod_z), 0.5f);

        // Render right model
        renderer.renderModel(shaderProg, VAO, fullVertexData, glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.1f), glm::vec3(theta_mod_x, theta_mod_y, theta_mod_z), 0.5f);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}