#ifndef LIGHTING_H
#define LIGHTING_H

#include <glm/glm.hpp>

class Lighting
{
public:
    enum class LightType
    {
        POINT,
        DIRECTIONAL
    };

    Lighting();
    Lighting(const glm::vec3& position, const glm::vec3& color, float ambientStrength, float specularStrength, float specularPhong, LightType type = LightType::POINT, const glm::vec3& direction = glm::vec3(0.0f, -1.0f, 0.0f));

    void setPosition(const glm::vec3& position);
    void setColor(const glm::vec3& color);
    void setAmbientStrength(float strength);
    void setSpecularStrength(float strength);
    void setSpecularPhong(float phong);
    void setDirection(const glm::vec3& direction);
    void setType(LightType type);

    glm::vec3 getPosition() const;
    glm::vec3 getColor() const;
    float getAmbientStrength() const;
    float getSpecularStrength() const;
    float getSpecularPhong() const;
    glm::vec3 getDirection() const;
    LightType getType() const;

private:
    glm::vec3 position;
    glm::vec3 color;
    float ambientStrength;
    float specularStrength;
    float specularPhong;
    glm::vec3 direction;
    LightType type;
};

#endif // LIGHTING_H
