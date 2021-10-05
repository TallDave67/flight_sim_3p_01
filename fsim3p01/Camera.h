#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "DrawingWindow.h"

class Camera
{
public:
    Camera();
    ~Camera();

    void initialize(DrawingWindow* _win, glm::vec3 _eye, glm::vec3 _worldUp, GLfloat _yaw, GLfloat _pitch, GLfloat _moveSpeed, GLfloat _angleSpeed);
    
    void handleKeyEvent();

    glm::vec3 getCameraEye();

    glm::mat4 getView();

private:
    DrawingWindow* win;

    glm::vec3 eye;
    glm::vec3 at;
    glm::vec3 worldUp;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;

    GLfloat yaw; //looking left and right (rotate around Y axis)
    GLfloat pitch; //looking up and down (rotate around X axis)

    GLfloat moveSpeed;
    GLfloat angleSpeed;

private:
    void update();
};
