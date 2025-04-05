#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include "Shader.h"
#include "Lighting.h"

class Renderer
{
public:
    Renderer(float windowWidth, float windowHeight);
    void renderSkybox(const Shader& skyboxShader, GLuint skyboxVAO, GLuint skyboxTex, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    void renderModel(const Shader& shader, GLuint VAO, const std::vector<GLfloat>& vertexData, const glm::vec3& position, const glm::vec3& scale, const glm::vec3& rotation, float alpha);
    void setCamera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up);
    void setLight(const Lighting& lighting);
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    glm::mat4 getViewMatrix() const;

private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPos;
    Lighting lighting;
};

#endif
