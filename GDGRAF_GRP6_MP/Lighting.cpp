#include "Lighting.h"

Lighting::Lighting()
    : position(0.0f, 0.0f, 0.0f), color(1.0f, 1.0f, 1.0f), ambientStrength(0.1f), specularStrength(0.5f), specularPhong(32.0f)
{
}

Lighting::Lighting(const glm::vec3& position, const glm::vec3& color, float ambientStrength, float specularStrength, float specularPhong)
    : position(position), color(color), ambientStrength(ambientStrength), specularStrength(specularStrength), specularPhong(specularPhong)
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
