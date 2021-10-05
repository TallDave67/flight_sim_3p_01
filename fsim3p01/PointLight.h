#pragma once

#include "Light.h"

class PointLight : public Light
{
public:
    PointLight();
    ~PointLight();

    void initialize(
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
        GLfloat x, GLfloat y, GLfloat z,
        GLfloat _constant,  GLfloat _linear, GLfloat _exponent);

    void UseLight(
        GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
        GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);

    void setPosition(glm::vec3 _position);

protected:
    glm::vec3 position;
    GLfloat constant;
    GLfloat linear;
    GLfloat exponent;
};

