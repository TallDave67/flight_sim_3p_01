#include "MotionPlan.h"

#include "Constants.h"

MotionPlan::MotionPlan() :
    position_start(0.0f, 0.0f, 0.0f), angle_start(0.0f), axis_of_rotation_start(0.0f, 0.0f, 0.0f), size_start(0.0f),
    speed_translate_x_start(0.0f), speed_translate_y_start(0.0f), speed_translate_z_start(0.0f),
    speed_rotate_start(0.0f), speed_scale_start(0.0f),
    motion_segments(nullptr), current_segment(0), repeat(false)
{
}

MotionPlan::~MotionPlan()
{
}

void MotionPlan::initialize(
    glm::vec3 _position_start, glm::vec3 _axis_of_rotation_start, float _angle_start, float _size_start, 
    float _speed_translate_x_start, float _speed_translate_y_start, float _speed_translate_z_start, 
    float _speed_rotate_start, float _speed_scale_start,
    std::vector<MotionSegment> *_motion_segments, bool _repeat)
{
    position_start = _position_start;
    axis_of_rotation_start = _axis_of_rotation_start;
    angle_start = _angle_start;
    size_start = _size_start;
    speed_translate_x_start = _speed_translate_x_start;
    speed_translate_y_start = _speed_translate_y_start;
    speed_translate_z_start = _speed_translate_z_start;
    speed_rotate_start = _speed_rotate_start;
    speed_scale_start = _speed_scale_start;
    motion_segments = _motion_segments;
    repeat =_repeat;

    motion.initialize(
        position_start, axis_of_rotation_start, angle_start, size_start,
        speed_translate_x_start, speed_translate_y_start, speed_translate_z_start, 
        speed_rotate_start, speed_scale_start);
}
    
void MotionPlan::move()
{
    if (next())
    {
        execute();
    }
    else if (repeat)
    {
        if (reset())
        {
            move();
        }
    }
}

bool MotionPlan::reset()
{
    motion.initialize(
        position_start, axis_of_rotation_start, angle_start, size_start, 
        speed_translate_x_start, speed_translate_y_start, speed_translate_z_start, 
        speed_rotate_start, speed_scale_start);

    current_segment = 0;
    unsigned int num_frames = 0;
    if (motion_segments)
    {
        std::vector<MotionSegment>::iterator itr_seg = motion_segments->begin();
        for (; itr_seg != motion_segments->end(); itr_seg++)
        {
            num_frames += itr_seg->reset();
        }
    }

    return (num_frames > 0);
}

bool MotionPlan::next()
{
    bool n = false;
    if (motion_segments)
    {
        while (current_segment < static_cast<int>(motion_segments->size()))
        {
            if (!(*motion_segments)[current_segment].next())
            {
                current_segment++;
            }
            else
            {
                n = true;
                break;
            }
        }
    }
    return n;
}

void MotionPlan::execute()
{
    (*motion_segments)[current_segment].execute();
    
    // Translation
    if ((*motion_segments)[current_segment].translate)
    {
        motion.translate(AXIS_X, (*motion_segments)[current_segment].direction_x);
        motion.translate(AXIS_Y, (*motion_segments)[current_segment].direction_y);
        motion.translate(AXIS_Z, (*motion_segments)[current_segment].direction_z);
    }

    // Rotation
    if ((*motion_segments)[current_segment].rotate)
    {
        motion.rotate((*motion_segments)[current_segment].axis_of_rotation, (*motion_segments)[current_segment].direction_rotate);
    }

    // Scaling
    if ((*motion_segments)[current_segment].scale)
    {
        motion.scale((*motion_segments)[current_segment].direction_scale);
    }
}

Motion* MotionPlan::get_motion()
{
    return &motion;
}