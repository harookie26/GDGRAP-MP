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

float brightness = 100.0f;

float lightX = 0;
float lightY = 0;
float lightZ = 0;

float lightDirX = 0;
float lightDirY = -1;
float lightDirZ = 0.5;

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

// Add point light positions for the cars
glm::vec3 car1LightPos(0, 0, 0);
glm::vec3 car2LightPos(0.5f, -0.5f, -0.5f);


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
	Shader carShader("Shaders/Car.vert", "Shaders/Car.frag");

	std::vector<std::string> facesMorningSkybox(morningSkybox, morningSkybox + sizeof(morningSkybox) / sizeof(std::string));
	std::vector<std::string> facesNightSkybox(nightSkybox, nightSkybox + sizeof(nightSkybox) / sizeof(std::string));

	Skybox morningSkyboxObj(facesMorningSkybox);
	Skybox nightSkyboxObj(facesNightSkybox);

	Skybox* currentSkybox = &morningSkyboxObj;

	Renderer renderer(windowWidth, windowHeight);

	Lighting morningLighting(glm::vec3(lightX, lightY, lightZ), glm::vec3(1.0f, 0.9f, 0.7f), 0.1f, 5.0f, 16.0f, Lighting::LightType::DIRECTIONAL, glm::vec3(lightDirX, lightDirY, lightDirZ)); // Adjusted direction vector
	Lighting nightLighting(glm::vec3(lightX, lightY, lightZ), glm::vec3(0.2f, 0.2f, 0.6f), 0.1f, 5.0f, 16.0f, Lighting::LightType::DIRECTIONAL, glm::vec3(lightDirX, lightDirY, lightDirZ)); // Adjusted direction vector

	// Add point lights for the cars
	//Lighting car1Light(car1LightPos, glm::vec3(1.0f, 0, 0), 0.1f, 1.0f, 1.0f, Lighting::LightType::POINT);
	//car1Light.setAttenuation(1.0f, 1.0f, 1.0f); // Increase these values to make the light very small

	ObjectLoader objectLoader;
	std::vector<GLfloat> fullVertexData;
	std::vector<GLuint> mesh_indices;
	objectLoader.loadObject("3D/Car.obj", fullVertexData, mesh_indices);

	GLuint texture = TextureLoader::loadTexture("3D/Road.png");
	GLuint norm_tex = TextureLoader::loadTexture("3D/Road_Normals.png");

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

		shaderProg.use();
		shaderProg.setInt("lightType", 1); // 1 for directional light
		shaderProg.setVec3("lightDir", glm::vec3(lightDirX, lightDirY, lightDirZ));

		morningLighting.setPosition(glm::vec3(lightX, lightY, lightZ));
		//renderer.setLight(morningLighting);

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
			renderer.setLight(morningLighting);
		}
		else if (InputHandler::currentSkybox == 1)
		{
			currentSkybox = &nightSkyboxObj;
			renderer.setLight(nightLighting);
		}

		currentSkybox->render(skyboxShaderProg, renderer.getViewMatrix(), projectionMatrix);

		/*// Update car light positions based on car positions
		car1Light.setPosition(glm::vec3(InputHandler::car_pos_x, InputHandler::car_pos_y, InputHandler::car_pos_z));

		// Set the lights in the renderer
		renderer.setLight(car1Light);

		carShader.use();
		// Set point light properties
		shaderProg.setVec3("pointLights[0].position", car1Light.getPosition());
		shaderProg.setVec3("pointLights[0].color", car1Light.getColor());
		shaderProg.setFloat("pointLights[0].ambientStrength", car1Light.getAmbientStrength());
		shaderProg.setFloat("pointLights[0].specularStrength", car1Light.getSpecularStrength());
		shaderProg.setFloat("pointLights[0].specularPhong", car1Light.getSpecularPhong());
		shaderProg.setFloat("pointLights[0].constant", 1.0f);
		shaderProg.setFloat("pointLights[0].linear", 0.09f);
		shaderProg.setFloat("pointLights[0].quadratic", 0.032f);*/

		carShader.setVec3("viewPos", cameraPos);


		renderer.renderModel(carShader, VAO, fullVertexData, glm::vec3(InputHandler::car_pos_x, InputHandler::car_pos_y, InputHandler::car_pos_z), glm::vec3(0.1f), glm::vec3(InputHandler::theta_mod_x, InputHandler::theta_mod_y, InputHandler::theta_mod_z), 1.0f);


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
		if (-100.00f >= InputHandler::car_pos_z)//||
		{
			std::cout << "FINISHED";
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
