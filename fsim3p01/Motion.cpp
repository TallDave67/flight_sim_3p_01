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

void Motion::compute_incremental_translation(int direction_x, int direction_y, int direction_z)
{
    float x = compute_translation_distance(direction_x, speed_translate_x);
    float y = compute_translation_distance(direction_y, speed_translate_y);
    float z = compute_translation_distance(direction_z, speed_translate_z);
    set_translation(x, y, z);
}

void Motion::apply_translation(glm::mat4& model)
{
    model = glm::translate(model, glm::vec3(position.x, position.y, position.z));
}

// Rotation
void Motion::set_rotation(float angle_x, float angle_y, float angle_z)
{
    glm::tmat4x4<float> next_rotation_matrix = glm::eulerAngleYZX(glm::radians(angle_y), glm::radians(angle_z), glm::radians(angle_x));
    rotation_matrix = next_rotation_matrix * rotation_matrix;
}

void Motion::compute_incremental_rotation(int direction_x, int direction_y, int direction_z)
{
    float angle_x = compute_rotation_angle(direction_x, speed_translate_x);
    float angle_y = compute_rotation_angle(direction_y, speed_translate_y);
    float angle_z = compute_rotation_angle(direction_z, speed_translate_z);
    set_rotation(angle_x, angle_y, angle_z);
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
    rotation_matrix = glm::eulerAngleYZX(0.0f, 0.0f, 0.0f);
    size = 0.0f;
}