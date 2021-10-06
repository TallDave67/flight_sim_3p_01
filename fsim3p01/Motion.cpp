#include "Motion.h"

#include <glm/gtc/matrix_transform.hpp>

#include "Constants.h"

Motion::Motion() :
    speed_translate_x(0.0f), speed_translate_y(0.0f), speed_translate_z(0.0f), 
    speed_rotate_x(0.0f), speed_rotate_y(0.0f), speed_rotate_z(0.0f), speed_scale(0.0f)
{
    reset_motion();
}

Motion::~Motion()
{
}

void Motion::initialize(
    float _speed_translate_x, float _speed_translate_y, float _speed_translate_z,
    float _speed_rotate_x, float _speed_rotate_y, float _speed_rotate_z, float _speed_scale)
{
    set_speed_translate_x(_speed_translate_x);
    set_speed_translate_y(_speed_translate_y);
    set_speed_translate_z(_speed_translate_z);
    set_speed_rotate_x(_speed_rotate_x);
    set_speed_rotate_y(_speed_rotate_y);
    set_speed_rotate_z(_speed_rotate_z);
    set_speed_scale(_speed_scale);
}

// Speeds
void Motion::set_speed_translate_x(float speed)
{
    speed_translate_x = speed;
}

void Motion::set_speed_translate_y(float speed)
{
    speed_translate_y = speed;
}

void Motion::set_speed_translate_z(float speed)
{
    speed_translate_z = speed;
}

void Motion::set_speed_rotate_x(float speed)
{
    speed_rotate_x = speed;
}

void Motion::set_speed_rotate_y(float speed)
{
    speed_rotate_y = speed;
}

void Motion::set_speed_rotate_z(float speed)
{
    speed_rotate_z = speed;
}

void Motion::set_speed_scale(float speed)
{
    speed_scale = speed;
}

// Translation
void Motion::set_translation(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
}

void Motion::compute_incremental_translation(int axes, int direction)
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if (axes & AXIS_X)
    {
        x = compute_translation_distance(direction, speed_translate_x);
    }
    if (axes & AXIS_Y)
    {
        y = compute_translation_distance(direction, speed_translate_y);
    }
    if (axes & AXIS_Z)
    {
        z = compute_translation_distance(direction, speed_translate_z);
    }
    set_translation(x, y, z);
}

void Motion::apply_translation(glm::mat4& model)
{
    model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
}

// Rotation
void Motion::set_rotation(int axes, float angle)
{
    glm::tmat4x4<float> next_rotation_matrix;
    if (axes & AXIS_X && axes == AXIS_Y && axes == AXIS_Z)
    {
        next_rotation_matrix = glm::eulerAngleXYZ(glm::radians(angle), glm::radians(angle), glm::radians(angle));
    }
    else if (axes & AXIS_X && axes == AXIS_Y)
    {
        next_rotation_matrix = glm::eulerAngleXY(glm::radians(angle), glm::radians(angle));
    }
    else if (axes & AXIS_X && axes == AXIS_Z)
    {
        next_rotation_matrix = glm::eulerAngleXZ(glm::radians(angle), glm::radians(angle));
    }
    else if (axes & AXIS_Y && axes == AXIS_Z)
    {
        next_rotation_matrix = glm::eulerAngleYZ(glm::radians(angle), glm::radians(angle));
    }
    else if (axes & AXIS_X)
    {
        next_rotation_matrix = glm::eulerAngleX(glm::radians(angle));
    }
    else if (axes & AXIS_Y)
    {
        next_rotation_matrix = glm::eulerAngleY(glm::radians(angle));
    }
    else if (axes & AXIS_Z)
    {
        next_rotation_matrix = glm::eulerAngleZ(glm::radians(angle));
    }
    rotation_matrix = next_rotation_matrix * rotation_matrix;
}

void Motion::compute_incremental_rotation(int axes, int direction)
{
    float angle = 0.0f;
    int num_axes = 0;
    if (axes & AXIS_X)
    {
        angle += compute_rotation_angle(direction, speed_translate_x);
        num_axes++;
    }
    if (axes & AXIS_Y)
    {
        angle += compute_rotation_angle(direction, speed_translate_y);
        num_axes++;
    }
    if (axes & AXIS_Z)
    {
        angle += compute_rotation_angle(direction, speed_translate_z);
        num_axes++;
    }
    if (num_axes)
    {
        // use average angle
        set_rotation(axes, angle / static_cast<float>(num_axes));
    }
}

void  Motion::apply_rotation(glm::mat4& model)
{
    model *= rotation_matrix;
}

glm::vec3 Motion::get_direction(glm::vec3 _position)
{
    glm::mat4 model(1.0f);
    model = model * rotation_matrix;
    glm::vec3 new_position = glm::vec3(model * glm::vec4(_position, 1.0));
    glm::vec3 direction = new_position - _position;
    //printf("Motion::getDirection() x = %2.4f, y = %2.4f, z = %2.4f\n", direction.x, direction.y, direction.z);
    return direction;
}

// Scaling
void Motion::set_scaling(float factor)
{
    size = factor;
}

void Motion::compute_incremental_scaling(int direction)
{
    float factor = 0.0f;
    factor = compute_scaling_factor(direction, speed_scale);
    size += factor;
}

void Motion::apply_scaling(glm::mat4& model)
{
    // only uniform scaling for now
    model = glm::scale(model, glm::vec3(size, size, size));
}

float Motion::compute_translation_distance(int direction, float speed)
{
    float distance = 0.0f;
    if (direction == DIRECTION_POSITIVE)
    {
        distance += (TRANSLATION_INCREMENT*speed);
        if (distance > TRANSLATION_MAX_OFFSET)
        {
            distance = TRANSLATION_MAX_OFFSET;
        }
    } 
    else if (direction == DIRECTION_NEGATIVE)
    {
        distance -= (TRANSLATION_INCREMENT*speed);
        if (distance < (-TRANSLATION_MAX_OFFSET))
        {
            distance = -TRANSLATION_MAX_OFFSET;
        }
    }
    return distance;
}

float Motion::compute_rotation_angle(int direction, float speed)
{
    float angle = 0.0f;
    if (direction == DIRECTION_POSITIVE)
    {
        angle += (ROTATION_INCREMENT*speed);
        if (angle > ROTATION_MAX_ANGLE)
        {
            angle -= ROTATION_MAX_ANGLE;
        }
    }
    else if (direction == DIRECTION_NEGATIVE)
    {
        angle -= (ROTATION_INCREMENT*speed);
        if (angle < (-ROTATION_MAX_ANGLE))
        {
            angle += ROTATION_MAX_ANGLE;
        }
    }
    return angle;
}

float Motion::compute_scaling_factor(int direction, float speed)
{
    float factor = 0.0f;
    if (direction == DIRECTION_POSITIVE)
    {
        factor += (SCALING_INCREMENT*speed);
        if (factor > SCALING_MAX_SIZE)
        {
            factor = SCALING_MAX_SIZE;
        }
    }
    else if (direction == DIRECTION_NEGATIVE)
    {
        factor -= (SCALING_INCREMENT*speed);
        if (factor < SCALING_MIN_SIZE)
        {
            factor = SCALING_MIN_SIZE;
        }
    }
    return factor;
}

void Motion::reset_motion()
{
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation_matrix = glm::eulerAngleXYZ(0.0f, 0.0f, 0.0f);
    size = 0.0f;
}