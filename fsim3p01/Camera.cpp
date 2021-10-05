#include "Camera.h"

Camera::Camera() :
    win(nullptr), eye(0.0f), at(0.0f), worldUp(0.0f), 
    front(0.0f), up(0.0f), right(0.0f),
    yaw(0.0f), pitch(0.0f), moveSpeed(0.0f), angleSpeed(0.0f)
{
}

Camera::~Camera()
{
}

void Camera::initialize(DrawingWindow* _win, glm::vec3 _eye, glm::vec3 _worldUp, GLfloat _yaw, GLfloat _pitch, GLfloat _moveSpeed, GLfloat _angleSpeed)
{
    win = _win;
    eye = _eye;
    worldUp = _worldUp;

    yaw = _yaw;
    pitch = _pitch;

    moveSpeed = _moveSpeed;
    angleSpeed = _angleSpeed;

    update();
}

void Camera::handleKeyEvent()
{
    if (win)
    {
        bool do_update = false;

        // Let user press multiple keys at the same time
        if (win->getKeyState(GLFW_KEY_W) || win->getKeyState(GLFW_KEY_Z))
        {
            // move camera forward (into screen)
            eye += front * moveSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_S))
        {
            // move camera backwards (out of screen)
            eye -= front * moveSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_A) || win->getKeyState(GLFW_KEY_Q))
        {
            // move camera left
            eye -= right * moveSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_D))
        {
            // move camera right
            eye += right * moveSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_UP))
        {
            // angle camera up
            pitch += angleSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_DOWN))
        {
            // angle camera down
            pitch -= angleSpeed;
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_LEFT))
        {
            // angle camera left
            yaw -= angleSpeed;         
            do_update = true;
        }
        if (win->getKeyState(GLFW_KEY_RIGHT))
        {
            // angle camera right
            yaw += angleSpeed;
            do_update = true;
        }

        if (do_update)
        {
            // fix any angle growing too big
            if(pitch > 89.0f)
                pitch =  89.0f;
            else if(pitch < -89.0f)
                 pitch = -89.0f; 
            update();
        }
    }
}

glm::vec3 Camera::getCameraEye()
{
    return eye;
}

glm::mat4 Camera::getView()
{
    return glm::lookAt(eye, at, worldUp);
}

void Camera::update()
{
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(front); //make a unit vector cause we just care about the direction

    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));

    // compute for later retreival
    at = eye + front;
    //printf("eye.x=%.6f, eye.y=%.6f, eye.z=%.6f, at.x=%.6f, at.y=%.6f, at.z=%.6f\n", eye.x, eye.y, eye.z, at.x, at.y, at.z);
}


