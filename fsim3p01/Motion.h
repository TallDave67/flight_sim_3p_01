#pragma once

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp> 

class Motion
{
public:
    friend class MotionPlan;

    Motion();
    ~Motion();

    void initialize(
        float _speed_translate_x, float _speed_translate_y, float _speed_translate_z,
        float _speed_rotate_x, float _speed_rotate_y, float _speed_rotate_z, float _speed_scale
    );

    // Speeds
    void set_speed_translate_x(float speed);
    void set_speed_translate_y(float speed);
    void set_speed_translate_z(float speed);
    void set_speed_rotate_x(float speed);
    void set_speed_rotate_y(float speed);
    void set_speed_rotate_z(float speed);
    void set_speed_scale(float speed);

    // Translation
    void set_translation(float x, float y, float z);
    void compute_incremental_translation(int direction_x, int direction_y, int direction_z);
    void apply_translation(glm::mat4 & model);

    // Rotation
    void set_rotation(float angle_x, float angle_y, float angle_z);
    void compute_incremental_rotation(int direction_x, int direction_y, int direction_z);
    void apply_rotation(glm::mat4 & model);
    glm::vec3 get_direction(glm::vec3 _position);

    // Scaling
    void set_scaling(float factor);
    void compute_incremental_scaling(int direction);
    void apply_scaling(glm::mat4 & model);

    virtual void reset_motion();
private:
    glm::vec3 position;
    glm::tmat4x4<float> rotation_matrix;
    float size;
    //
    float speed_translate_x;
    float speed_translate_y;
    float speed_translate_z;
    float speed_rotate_x;
    float speed_rotate_y;
    float speed_rotate_z;
    float speed_scale;

private:
    float compute_translation_distance(int direction, float speed);
    float compute_rotation_angle(int direction, float speed);
    float compute_scaling_factor(int direction, float speed);
};

