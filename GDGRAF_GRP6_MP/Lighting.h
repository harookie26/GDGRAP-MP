#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>

class Lighting
{
public:
    Lighting();
    Lighting(const glm::vec3& position, const glm::vec3& color, float ambientStrength, float specularStrength, float specularPhong);

    void setPosition(const glm::vec3& position);
    void setColor(const glm::vec3& color);
    void setAmbientStrength(float strength);
    void setSpecularStrength(float strength);
    void setSpecularPhong(float phong);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    float getAmbientStrength() const;
    float getSpecularStrength() const;
    float getSpecularPhong() const;

private:
    glm::vec3 position;
    glm::vec3 color;
    float ambientStrength;
    float specularStrength;
    float specularPhong;
};

#endif
