#pragma once

#include <stdio.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class DrawingWindow
{
public:
    DrawingWindow();
    DrawingWindow(GLint _width, GLint _height);
    ~DrawingWindow();

    int initialize();

    GLint getWidth();
    GLint getHeight();

    GLint getBufferWidth();
    GLint getBufferHeight();

    bool shouldClose();
    void swapBuffers();

    bool getKeyState(int key);
    GLfloat getMouseChangeX();
    GLfloat getMouseChangeY();

private:
    GLFWwindow* win;

    GLint width;
    GLint height;

    GLint bufferWidth;
    GLint bufferHeight;

    std::vector<bool> keys;


private:
    void registerCallbacks();
    bool isValidKey(int key);

    static void handleKeys(GLFWwindow* win, int key, int code, int action, int mode);
};

