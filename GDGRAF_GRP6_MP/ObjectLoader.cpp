#include "ObjectLoader.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>

bool ObjectLoader::loadObject(const std::string& path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warning, error;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error, path.c_str());
    if (!success) {
        std::cerr << "Failed to load object: " << error << std::endl;
        return false;
    }

    std::vector<glm::vec3> tangents, bitangents;
    calculateTangentsAndBitangents(attributes, shapes, tangents, bitangents);

    for (size_t i = 0; i < shapes[0].mesh.indices.size(); ++i) {
        tinyobj::index_t vData = shapes[0].mesh.indices[i];

        vertexData.push_back(attributes.vertices[3 * vData.vertex_index + 0]);
        vertexData.push_back(attributes.vertices[3 * vData.vertex_index + 1]);
        vertexData.push_back(attributes.vertices[3 * vData.vertex_index + 2]);

        vertexData.push_back(attributes.normals[3 * vData.normal_index + 0]);
        vertexData.push_back(attributes.normals[3 * vData.normal_index + 1]);
        vertexData.push_back(attributes.normals[3 * vData.normal_index + 2]);

        vertexData.push_back(attributes.texcoords[2 * vData.texcoord_index + 0]);
        vertexData.push_back(attributes.texcoords[2 * vData.texcoord_index + 1]);

        vertexData.push_back(tangents[i].x);
        vertexData.push_back(tangents[i].y);
        vertexData.push_back(tangents[i].z);

        vertexData.push_back(bitangents[i].x);
        vertexData.push_back(bitangents[i].y);
        vertexData.push_back(bitangents[i].z);

        indices.push_back(vData.vertex_index);
    }

    return true;
}

void ObjectLoader::calculateTangentsAndBitangents(const tinyobj::attrib_t& attributes, const std::vector<tinyobj::shape_t>& shapes, std::vector<glm::vec3>& tangents, std::vector<glm::vec3>& bitangents)
{
    for (size_t i = 0; i < shapes[0].mesh.indices.size(); i += 3) {
        tinyobj::index_t vData1 = shapes[0].mesh.indices[i];
        tinyobj::index_t vData2 = shapes[0].mesh.indices[i + 1];
        tinyobj::index_t vData3 = shapes[0].mesh.indices[i + 2];

        glm::vec3 v1 = glm::vec3(
            attributes.vertices[3 * vData1.vertex_index + 0],
            attributes.vertices[3 * vData1.vertex_index + 1],
            attributes.vertices[3 * vData1.vertex_index + 2]
        );

        glm::vec3 v2 = glm::vec3(
            attributes.vertices[3 * vData2.vertex_index + 0],
            attributes.vertices[3 * vData2.vertex_index + 1],
            attributes.vertices[3 * vData2.vertex_index + 2]
        );

        glm::vec3 v3 = glm::vec3(
            attributes.vertices[3 * vData3.vertex_index + 0],
            attributes.vertices[3 * vData3.vertex_index + 1],
            attributes.vertices[3 * vData3.vertex_index + 2]
        );

        glm::vec2 uv1 = glm::vec2(
            attributes.texcoords[2 * vData1.texcoord_index + 0],
            attributes.texcoords[2 * vData1.texcoord_index + 1]
        );
        glm::vec2 uv2 = glm::vec2(
            attributes.texcoords[2 * vData2.texcoord_index + 0],
            attributes.texcoords[2 * vData2.texcoord_index + 1]
        );
        glm::vec2 uv3 = glm::vec2(
            attributes.texcoords[2 * vData3.texcoord_index + 0],
            attributes.texcoords[2 * vData3.texcoord_index + 1]
        );

        glm::vec3 deltaPos1 = v2 - v1;
        glm::vec3 deltaPos2 = v3 - v1;
        glm::vec2 deltaUV1 = uv2 - uv1;
        glm::vec2 deltaUV2 = uv3 - uv1;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        tangents.push_back(tangent);
        tangents.push_back(tangent);
        tangents.push_back(tangent);

        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
        bitangents.push_back(bitangent);
    }
}
