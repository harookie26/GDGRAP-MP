#include "Lighting.h"

Lighting::Lighting()
    : position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), ambientStrength(0.1f), specularStrength(0.5f),
    specularPhong(32.0f), direction(0.0f, -1.0f, 0.0f), type(LightType::POINT)
{
}

Lighting::Lighting(const glm::vec3& position, const glm::vec3& color, float ambientStrength, float specularStrength,
    float specularPhong, LightType type, const glm::vec3& direction)
    : position(position), color(color), ambientStrength(ambientStrength), specularStrength(specularStrength),
    specularPhong(specularPhong), direction(direction), type(type)
{
}

void Lighting::setPosition(const glm::vec3& position)
{
    this->position = position;
}

void Lighting::setColor(const glm::vec3& color)
{
    this->color = color;
}

void Lighting::setAmbientStrength(float strength)
{
    this->ambientStrength = strength;
}

void Lighting::setSpecularStrength(float strength)
{
    this->specularStrength = strength;
}

void Lighting::setSpecularPhong(float phong)
{
    this->specularPhong = phong;
}

void Lighting::setDirection(const glm::vec3& direction)
{
    this->direction = direction;
}

void Lighting::setType(LightType type)
{
    this->type = type;
}

void Lighting::setAttenuation(float constant, float linear, float quadratic)
{
    this->constantAttenuation = constant;
    this->linearAttenuation = linear;
    this->quadraticAttenuation = quadratic;
}

glm::vec3 Lighting::getPosition() const
{
    return position;
}

glm::vec3 Lighting::getColor() const
{
    return color;
}

float Lighting::getAmbientStrength() const
{
    return ambientStrength;
}

float Lighting::getSpecularStrength() const
{
    return specularStrength;
}

float Lighting::getSpecularPhong() const
{
    return specularPhong;
}

glm::vec3 Lighting::getDirection() const
{
    return direction;
}

Lighting::LightType Lighting::getType() const
{
    return type;
}
