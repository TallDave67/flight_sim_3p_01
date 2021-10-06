#pragma once

#include <glm/glm.hpp>

class MotionSegment
{
public:
    friend class MotionPlan;

    MotionSegment(
        unsigned int _num_frames, unsigned int _current_frame,
        bool _translate, int _direction_x, int _direction_y, int _direction_z,
        bool _rotate, int _direction_rotate_x, int _direction_rotate_y, int _direction_rotate_z,
        bool _scale, int _direction_scale
    );
    ~MotionSegment();

    unsigned int reset();
    bool next();
    void execute();

private:
    // Duration
    unsigned int num_frames;
    unsigned int current_frame;

    // Translation
    bool translate;
    int direction_x;
    int direction_y;
    int direction_z;

    // Rotation
    bool rotate;
    int direction_rotate_x;
    int direction_rotate_y;
    int direction_rotate_z;

    // Scaling
    bool scale;
    int direction_scale;
};

