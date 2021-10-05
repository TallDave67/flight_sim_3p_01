#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material();
    ~Material();

    void initialize(GLfloat _specularIntensity, GLfloat _shininess);

    void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

private:
    GLfloat specularIntensity;
    GLfloat shininess;
};

