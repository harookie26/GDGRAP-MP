#ifndef OBJECTLOADER_H
#define OBJECTLOADER_H

#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "tiny_obj_loader.h"

class ObjectLoader
{
public:
	bool loadObject(const std::string& path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices);

private:
	void calculateTangentsAndBitangents(const tinyobj::attrib_t& attributes,
	                                    const std::vector<tinyobj::shape_t>& shapes, std::vector<glm::vec3>& tangents,
	                                    std::vector<glm::vec3>& bitangents);
};

#endif // OBJECTLOADER_H
