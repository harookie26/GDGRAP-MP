#include "Renderer.h"

Renderer::Renderer(float windowWidth, float windowHeight)
{
    projectionMatrix = glm::perspective(glm::radians(60.0f), windowWidth / windowHeight, 0.1f, 100.0f);
}

void Renderer::renderSkybox(const Shader& skyboxShader, GLuint skyboxVAO, GLuint skyboxTex, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix)
{
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    glm::mat4 sky_view = glm::mat4(glm::mat3(viewMatrix));

    skyboxShader.setMat4("projection", projectionMatrix);
    skyboxShader.setMat4("view", sky_view);

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTex);

    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
}

void Renderer::renderModel(const Shader& shader, GLuint VAO, const std::vector<GLfloat>& vertexData, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, float alpha)
{
    shader.use();

    glm::mat4 transformation_matrix = glm::mat4(1.0f);
    transformation_matrix = glm::translate(transformation_matrix, position);
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transformation_matrix = glm::rotate(transformation_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    transformation_matrix = glm::scale(transformation_matrix, scale);

    shader.setMat4("transform", transformation_matrix);
    shader.setMat4("view", viewMatrix);
    shader.setMat4("projection", projectionMatrix);
    shader.setFloat("alpha", alpha);
    shader.setVec3("lightPos", lightPos);
    shader.setVec3("lightColor", lightColor);
    shader.setVec3("cameraPos", cameraPos);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertexData.size() / 14);
}


void Renderer::setCamera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up)
{
    cameraPos = position;
    viewMatrix = glm::lookAt(position, position + front, up);
}

void Renderer::setLight(const glm::vec3& position, const glm::vec3& color)
{
    lightPos = position;
    lightColor = color;
}

void Renderer::setProjectionMatrix(const glm::mat4& projectionMatrix)
{
    this->projectionMatrix = projectionMatrix;
}

glm::mat4 Renderer::getViewMatrix() const
{
    return viewMatrix;
}