#pragma once

#include <glm/glm.hpp>

class Motion
{
public:
    friend class MotionPlan;

    Motion();
    ~Motion();

    void initialize(
        glm::vec3 _position, glm::vec3 _axis_of_rotation, float _angle, float _size,
        float _speed_translate_x, float _speed_translate_y, float _speed_translate_z,
        float _speed_rotate, float _speed_scale
    );

    // Translation
    void translate(int axis, int direction);
    glm::vec3 getPosition();
    float getX();
    float getY();
    float getZ();

    // Rotation
    void rotate(glm::vec3 _axis_of_rotation, int direction);
    glm::vec3 getAxisOfRotation();
    float getAngle();
    glm::vec3 getDirection(glm::vec3 _position);

    // Scaling
    void scale(int direction);
    float getSize();

    // Speeds
    float get_speed_translate_x();
    void set_speed_translate_x(float speed);
    float get_speed_translate_y();
    void set_speed_translate_y(float speed);
    float get_speed_translate_z();
    void set_speed_translate_z(float speed);
    float get_speed_rotate();
    void set_speed_rotate(float speed);
    float get_speed_scale();
    void set_speed_scale(float speed);

private:
    glm::vec3 position;
    glm::vec3 axis_of_rotation;
    float angle;
    float size;
    //
    float speed_translate_x;
    float speed_translate_y;
    float speed_translate_z;
    float speed_rotate;
    float speed_scale;

private:
    void do_translate(float& value, int direction, float speed);
};

