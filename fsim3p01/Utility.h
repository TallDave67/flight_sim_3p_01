#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <GL/glew.h>

class Utility
{
public:
    Utility();
    ~Utility();

    void CalculateNormals(GLfloat* vertices, unsigned int num_vertices);
    void debugFragmentShader_ApplyLight();
    void axis_angle_to_euler(float x, float y, float z, float angle, float& heading, float& attitude, float& bank);
};

