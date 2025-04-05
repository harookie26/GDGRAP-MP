// Model.h
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

class Model {
public:
    Model(glm::vec3 position);
    
    void draw(GLuint* shaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData);

	void setTexture(GLuint* textureID, std::string texturePath);//testing


    glm::vec3 getPosition();


    void rotate(char axis, int direction);
	void translate(float x, float y, float z);
	void scale(float x, float y, float z);


private:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float axX;
    float axY;
    float axZ;

    glm::mat4 identity_matrix4;

  //w  std::vector<GLuint> mesh_indices;
    /*
    * 
    * glm::mat4 projection;
	glm::vec3 cameraPos;
	glm::vec3 WorldUp;
	glm::vec3 Front;
	glm::mat4 viewMatrix;   unsure if needed or not
    * 
    */


 
};

