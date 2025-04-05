#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "Shader.h"

class Skybox
{
public:
    Skybox(const std::vector<std::string>& faces);
    void render(const Shader& shader, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix);
    GLuint getVAO() const { return VAO; }
    GLuint getTextureID() const { return textureID; }

private:
    GLuint VAO, VBO, EBO;
    GLuint textureID;
    void loadCubemap(const std::vector<std::string>& faces);
};

#endif
