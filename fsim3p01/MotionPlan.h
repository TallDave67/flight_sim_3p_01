#pragma once
#include<vector>

#include <glm/glm.hpp>

#include "Motion.h"
#include "MotionSegment.h"

class MotionPlan
{
public:
    MotionPlan();
    ~MotionPlan();

    void initialize(
        glm::vec3 _position_start, glm::vec3 _axis_of_rotation_start, float _angle_start, float _size_start, 
        float _speed_translate_x_start, float _speed_translate_y_start, float _speed_translate_z_start,
        float _speed_rotate_start, float _speed_scale_start,
        std::vector<MotionSegment> *_motion_segments, bool _repeat);

    void move();

    bool reset();
    bool next();
    void execute();

    Motion* get_motion();

private:
    Motion motion;
    glm::vec3 position_start;
    glm::vec3 axis_of_rotation_start;
    float angle_start;
    float size_start;
    //
    float speed_translate_x_start;
    float speed_translate_y_start;
    float speed_translate_z_start;
    float speed_rotate_start;
    float speed_scale_start;

    std::vector<MotionSegment> *motion_segments;
    int current_segment;
    bool repeat;
};

