#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
    DirectionalLight();
    ~DirectionalLight();

    void initialize(
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
        GLfloat x, GLfloat y, GLfloat z);

    void UseLight(
        GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
        GLuint directionLocation);

private:
    glm::vec3 direction;
};

