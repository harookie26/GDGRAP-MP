#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <string>

class TextureLoader
{
public:
    static GLuint loadTexture(const std::string& path, bool flipVertically = true);
};

#endif // TEXTURELOADER_H
