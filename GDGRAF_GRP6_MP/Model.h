// Model.h
#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <string>

#include "ObjectLoader.h"

class Model
{
public:
	Model(glm::vec3 position);
	~Model();

	void draw(GLuint shaderProg);

	void setTexture(GLuint textureID);

	glm::vec3 getPosition() const;
	glm::vec3 getScale() const;
	glm::vec3 getRotation() const;
	float getAlpha() const;
	GLuint getVAO() const;
	const std::vector<GLfloat>& getVertexData() const;

	void rotate(char axis, float angle);
	void translate(float x, float y, float z);
	void setScale(float x, float y, float z); // Renamed from scale to setScale
	void setAlpha(float alpha); // New method to set alpha value

	bool loadObject(const std::string& path);
	void setupBuffers();

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
	float alpha; // New member to store alpha value
	float axX;
	float axY;
	float axZ;

	glm::mat4 identity_matrix4;

	std::vector<GLfloat> fullVertexData;
	std::vector<GLuint> mesh_indices;

	GLuint VAO, VBO, EBO;
	GLuint textureID;
};




