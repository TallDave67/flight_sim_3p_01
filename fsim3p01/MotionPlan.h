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
        std::vector<MotionSegment> *_motion_segments, bool _repeat,
        float _speed_translate_x, float _speed_translate_y, float _speed_translate_z,
        float _speed_rotate_x, float _speed_rotate_y, float _speed_rotate_z, float _speed_scale
    );

    void move();

    bool reset();
    bool next();
    void execute();

    Motion* get_motion();
    void set_callback(void (*_reset_motion)(Motion*));

private:
    Motion motion;
    void (*reset_motion)(Motion*);
    std::vector<MotionSegment> *motion_segments;
    int current_segment;
    bool repeat;
};

