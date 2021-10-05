#include "Motion.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

Motion::Motion() :
    position(TRANSLATION_OFFSET_START, TRANSLATION_OFFSET_START, TRANSLATION_OFFSET_START), 
    axis_of_rotation(0.0f, 0.0f, 0.0f), angle(ROTATION_ANGLE_START), size(SCALING_SIZE_START),
    speed_translate_x(0.0f), speed_translate_y(0.0f), speed_translate_z(0.0f), 
    speed_rotate(0.0f), speed_scale(0.0f)
{
}

Motion::~Motion()
{
}

void Motion::initialize(
    glm::vec3 _position, glm::vec3 _axis_of_rotation, float _angle, float _size,
    float _speed_translate_x,float _speed_translate_y, float _speed_translate_z,
    float _speed_rotate, float _speed_scale
)
{
    position = _position;
    axis_of_rotation = _axis_of_rotation;
    angle = _angle;
    size = _size;
    //printf("Motion::initialize() rotX=%2.6f, rotY=%2.6f, rotZ=%2.6f, angle=%2.6f\n", axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z, angle);

    set_speed_translate_x(_speed_translate_x);
    set_speed_translate_y(_speed_translate_y);
    set_speed_translate_z(_speed_translate_z);
    set_speed_rotate(_speed_rotate);
    set_speed_scale(_speed_scale);
}

// Translation
void Motion::translate(int axis, int direction)
{
    if (axis == AXIS_X)
    {
        //printf("Motion::translate() X\n");
        do_translate(position.x, direction, speed_translate_x);
    }
    else if (axis == AXIS_Y)
    {
        //printf("Motion::translate() Y\n");
        do_translate(position.y, direction, speed_translate_y);
    }
    else if (axis == AXIS_Z)
    {
        //printf("Motion::translate() Z\n");
        do_translate(position.z, direction, speed_translate_z);
    }
}

glm::vec3 Motion::getPosition()
{
    //printf("Motion::getPosition() x = %2.4f, y = %2.4f, z = %2.4f\n", position.x, position.y, position.z);
    return position;
}

float Motion::getX()
{
    //printf("Motion::getX() %2.6f\n", position.x);
    return position.x;
}

float Motion::getY()
{
    //printf("Motion::getY() %2.6f\n", position.y);
    return position.y;
}

float Motion::getZ()
{
    //printf("Motion::getZ() %2.6f\n", position.z);
    return position.z;
}

// Rotation
void Motion::rotate(glm::vec3 _axis_of_rotation, int direction)
{
    axis_of_rotation = _axis_of_rotation;
    //printf("Motion::rotate() X=%2.6f, Y=%2.6f, Z=%2.6f\n", axis_of_rotation.x, axis_of_rotation.y, axis_of_rotation.z);

    if (direction == DIRECTION_POSITIVE)
    {
        angle += (ROTATION_INCREMENT*speed_rotate);
        if (angle > ROTATION_MAX_ANGLE)
        {
            angle -= ROTATION_MAX_ANGLE;
        }
    }
    else if (direction == DIRECTION_NEGATIVE)
    {
        angle -= (ROTATION_INCREMENT*speed_rotate);
        if (angle < (-ROTATION_MAX_ANGLE))
        {
            angle += ROTATION_MAX_ANGLE;
        }
    }
}

glm::vec3 Motion::getAxisOfRotation()
{
    return axis_of_rotation;
}

float Motion::getAngle()
{
    return angle;
}

glm::vec3 Motion::getDirection(glm::vec3 _position)
{
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(angle), axis_of_rotation);
    glm::vec3 new_position = glm::vec3(model * glm::vec4(_position, 1.0));
    glm::vec3 direction = new_position - _position;
    //printf("Motion::getDirection() x = %2.4f, y = %2.4f, z = %2.4f\n", direction.x, direction.y, direction.z);
    return direction;
}

// Scaling
void Motion::scale(int direction)
{
    if (direction == DIRECTION_POSITIVE)
    {
        size += (SCALING_INCREMENT*speed_scale);
        if (size > SCALING_MAX_SIZE)
        {
            size = SCALING_MAX_SIZE;
        }
    }
    else if (direction == DIRECTION_NEGATIVE)
    {
        size -= (SCALING_INCREMENT*speed_scale);
        if (size < SCALING_MIN_SIZE)
        {
            size = SCALING_MIN_SIZE;
        }
    }
}

float Motion::getSize()
{
    return size;
}

float Motion::get_speed_translate_x()
{
    return speed_translate_x;
}

void Motion::set_speed_translate_x(float speed)
{
    speed_translate_x = speed;
}

float Motion::get_speed_translate_y()
{
    return speed_translate_y;
}

void Motion::set_speed_translate_y(float speed)
{
    speed_translate_y = speed;
}

float Motion::get_speed_translate_z()
{
    return speed_translate_z;
}

void Motion::set_speed_translate_z(float speed)
{
    speed_translate_z = speed;
}

float Motion::get_speed_rotate()
{
    return speed_rotate;
}

void Motion::set_speed_rotate(float speed)
{
    speed_rotate = speed;
}

float Motion::get_speed_scale()
{
    return speed_scale;
}

void Motion::set_speed_scale(float speed)
{
    speed_scale = speed;
}

void Motion::do_translate(float& value, int direction, float speed)
{
    if (direction == DIRECTION_POSITIVE)
    {
        value += (TRANSLATION_INCREMENT*speed);
        if (value > TRANSLATION_MAX_OFFSET)
        {
            value = TRANSLATION_MAX_OFFSET;
        }
    } 
    else if (direction == DIRECTION_NEGATIVE)
    {
        value -= (TRANSLATION_INCREMENT*speed);
        if (value < (-TRANSLATION_MAX_OFFSET))
        {
            value = -TRANSLATION_MAX_OFFSET;
        }
    }
    //printf("Motion::do_translate() %2.6f\n", value);
}