#include "DrawingWindow.h"

#include "Constants.h"

DrawingWindow::DrawingWindow() :
    win(nullptr), width(WINDOW_WIDTH), height(WINDOW_HEIGHT), bufferWidth(0), bufferHeight(0), keys(GLFW_KEY_LAST+1, false)
{
}

DrawingWindow::DrawingWindow(GLint _width, GLint _height) : DrawingWindow()
{
    width = _width;
    height = _height;
}

DrawingWindow:: ~DrawingWindow()
{
    glfwDestroyWindow(win);
    glfwTerminate();
}

int DrawingWindow::initialize()
{
    // Initialise GLFW
    if (!glfwInit())
    {
        printf("GLFW initialisation failed!\n");
        glfwTerminate();
        return 1;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Core profile = No Backwards Compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);;
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // the window into which we draw
    win = glfwCreateWindow(width, height, "OpenGL Main Drawing Window", NULL, NULL);
    if (!win)
    {
        printf("GLFW window creation failed!\n");
        glfwTerminate();
        return 1;
    }

    // Get buffer size information
    glfwGetFramebufferSize(win, &bufferWidth, &bufferHeight);

    // Set context for GLEW to use
    glfwMakeContextCurrent(win);

    // Now that GLEW has the context we can get the OpenGL version
    const GLubyte* versionInfo = glGetString(GL_VERSION);
    GLint majorVersionParams;
    glGetIntegerv(GL_MAJOR_VERSION, &majorVersionParams);
    GLint minorVersionParams;
    glGetIntegerv(GL_MINOR_VERSION, &minorVersionParams);
    printf("OpenGL: version info %s, major version %d, minor version %d\n", versionInfo, majorVersionParams, minorVersionParams);

    // Register our callbacks
    registerCallbacks();

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        printf("GLEW initialization failed!\n");
        glfwDestroyWindow(win);
        glfwTerminate();
        return 1;
    }

    // We need to know which shapes are in front of other shapes
    glEnable(GL_DEPTH_TEST);

    // Setup Viewport size
    glViewport(0, 0, bufferWidth, bufferHeight);

    // We do not disable the mouse cursor using GLFW_CURSOR_DISABLED
    // Because this disables the mouse for all other applications running on the system
    // This application is not more important than all the other running applications 
    // Therefore we simply hide the mouse cursor
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // Set the callback to the correct instance of this drawing window
    glfwSetWindowUserPointer(win, this);

    return 0;
}

GLint DrawingWindow::getWidth()
{
    return width;
}

GLint DrawingWindow::getHeight()
{
    return height;
}

GLint DrawingWindow::getBufferWidth()
{
    return bufferWidth;
}

GLint DrawingWindow::getBufferHeight()
{
    return bufferHeight;
}

bool DrawingWindow::shouldClose()
{
    return glfwWindowShouldClose(win);
}

void DrawingWindow::swapBuffers()
{
    glfwSwapBuffers(win);
}

bool DrawingWindow::isValidKey(int key)
{
    bool valid = false;

    // Check that key lies within numeric range
    if (key >= 0 && key <= GLFW_KEY_LAST)
    {
        valid = true;
    }

    return valid;
}

void DrawingWindow::registerCallbacks()
{
    glfwSetKeyCallback(win, handleKeys);
}

bool DrawingWindow::getKeyState(int key)
{
    bool state = false;

    // Is this a valid key?
    if (isValidKey(key))
    {
        state = keys[key];
    }

    return state;
}

// static functions
void DrawingWindow::handleKeys(GLFWwindow* win, int key, int code, int action, int mode)
{
    // Get our drawing window object
    DrawingWindow* drawingWindow = static_cast<DrawingWindow*>(glfwGetWindowUserPointer(win));

    // Is this a valid key?
    if (drawingWindow->isValidKey(key))
    {
        // Did the user just press the key?
        //printf("key = %d, action = %d\n", key, action);
        if (action == GLFW_PRESS)
        {
            // Examine the key pressed
            if (key == GLFW_KEY_ESCAPE)
            {
                glfwSetWindowShouldClose(win, GL_TRUE);
            }
            else
            {
                drawingWindow->keys[key] = true;
            }
        }
        // Or did the user just release the key?
        // Did the user just press the key?
        if (action == GLFW_RELEASE)
        {
            drawingWindow->keys[key] = false;
        }
    }
}
