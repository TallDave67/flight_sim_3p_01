#pragma once

#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight();
    ~SpotLight();

    void initialize(
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
        GLfloat x, GLfloat y, GLfloat z,
        GLfloat _constant,  GLfloat _linear, GLfloat _exponent,
        GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat _edge);

    void UseLight(
        GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
        GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint directionLocation, GLuint edgeLocation);

    void setDirection(glm::vec3 _direction);

private:
    glm::vec3 direction;
    GLfloat edge;
    GLfloat processedEdge;
};

