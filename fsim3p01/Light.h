#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light();
    ~Light();

    void initialize(
        GLfloat red, GLfloat green, GLfloat blue,
        GLfloat _ambientIntensity, GLfloat _diffuseIntensity);

    void UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation);

protected:
    glm::vec3 colour;
    GLfloat ambientIntensity;
    GLfloat diffuseIntensity;
};

