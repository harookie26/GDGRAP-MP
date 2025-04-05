// Model.cpp
#include "Model.h"

Model::Model(glm::vec3 position) : position(position)
{
	identity_matrix4 = glm::mat4(1.0f);

	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	axX = 0.0f;
	axY = 0.0f; // Fixed typo: axX to axY
	axZ = 0.0f;
}

void Model::draw(GLuint* shaderProg, GLuint* VAO, std::vector<GLfloat>* fullVertexData)
{
	glBindBuffer(GL_ARRAY_BUFFER, *VAO);
	glBufferData(GL_ARRAY_BUFFER, fullVertexData->size() * sizeof(GLfloat), &(*fullVertexData)[0], GL_STATIC_DRAW);

	glUseProgram(*shaderProg);

	// Set the model matrix
	glm::mat4 model = glm::translate(identity_matrix4, position);

	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	GLint modelLoc = glGetUniformLocation(*shaderProg, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(*VAO);
	glDrawArrays(GL_TRIANGLES, 0, fullVertexData->size() / 8); // Assuming each vertex has 8 attributes
}

void Model::setScale(float x, float y, float z) // Renamed from scale to setScale
{
	scale = glm::vec3(x, y, z);
}
