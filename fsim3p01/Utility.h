#pragma once

#include <GL/glew.h>

class Utility
{
public:
    Utility();
    ~Utility();

    void CalculateNormals(GLfloat* vertices, unsigned int num_vertices);
    void debugFragmentShader_ApplyLight();
};

