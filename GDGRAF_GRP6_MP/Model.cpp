// Model.cpp
#include "Model.h"

Model::Model(glm::vec3 position) : position(position), textureID(0), alpha(1.0f) // Initialize alpha to 1.0f
{
	identity_matrix4 = glm::mat4(1.0f);

	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	axX = 0.0f;
	axY = 0.0f; // Fixed typo: axX to axY
	axZ = 0.0f;
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Model::draw(GLuint shaderProg)
{
	glBindVertexArray(VAO);
	glUseProgram(shaderProg);

	// Bind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProg, "texture1"), 0);

	// Set the model matrix
	glm::mat4 model = glm::translate(identity_matrix4, position);

	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	GLint modelLoc = glGetUniformLocation(shaderProg, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// Set the alpha value
	GLint alphaLoc = glGetUniformLocation(shaderProg, "alpha");
	glUniform1f(alphaLoc, alpha);

	glDrawArrays(GL_TRIANGLES, 0, fullVertexData.size() / 14); // Assuming each vertex has 14 attributes
}

void Model::setScale(float x, float y, float z) // Renamed from scale to setScale
{
	scale = glm::vec3(x, y, z);
}

bool Model::loadObject(const std::string& path)
{
	ObjectLoader objectLoader;
	return objectLoader.loadObject(path, fullVertexData, mesh_indices);
}

void Model::setupBuffers()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_STATIC_DRAW);

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
}

void Model::rotate(char axis, float angle)
{
	switch (axis)
	{
	case 'x':
		rotation.x += angle;
		break;
	case 'y':
		rotation.y += angle;
		break;
	case 'z':
		rotation.z += angle;
		break;
	default:
		break;
	}
}

void Model::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
}

void Model::setTexture(GLuint textureID)
{
	this->textureID = textureID;
}

glm::vec3 Model::getPosition() const // Implementation of getPosition
{
	return position;
}

glm::vec3 Model::getScale() const // Implementation of getScale
{
	return scale;
}

glm::vec3 Model::getRotation() const // Implementation of getRotation
{
	return rotation;
}

float Model::getAlpha() const // Implementation of getAlpha
{
	return alpha;
}

GLuint Model::getVAO() const // Implementation of getVAO
{
	return VAO;
}

const std::vector<GLfloat>& Model::getVertexData() const // Implementation of getVertexData
{
	return fullVertexData;
}

void Model::setAlpha(float alpha) // Implementation of setAlpha
{
	this->alpha = alpha;
}


