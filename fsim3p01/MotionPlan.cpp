#include "MotionPlan.h"

#include "Constants.h"

MotionPlan::MotionPlan() :
    motion_segments(nullptr), current_segment(0), repeat(false), reset_motion(nullptr)
{
}

MotionPlan::~MotionPlan()
{
}

void MotionPlan::initialize(
        std::vector<MotionSegment> *_motion_segments, bool _repeat,
        float _speed_translate_x, float _speed_translate_y, float _speed_translate_z,
        float _speed_rotate_x, float _speed_rotate_y, float _speed_rotate_z, float _speed_scale
)
{
    motion_segments = _motion_segments;
    repeat =_repeat;

    motion.initialize(
        _speed_translate_x, _speed_translate_y, _speed_translate_z, 
        _speed_rotate_x, _speed_rotate_y, _speed_rotate_z, _speed_scale
    );
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
    motion.reset_motion();
    if (reset_motion)
    {
        reset_motion(&motion);
    }

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
        motion.compute_incremental_translation(
            (*motion_segments)[current_segment].direction_x, 
            (*motion_segments)[current_segment].direction_y,
            (*motion_segments)[current_segment].direction_z);
    }

    // Rotation
    if ((*motion_segments)[current_segment].rotate)
    {
        motion.compute_incremental_rotation(
            (*motion_segments)[current_segment].direction_rotate_x,
            (*motion_segments)[current_segment].direction_rotate_y,
            (*motion_segments)[current_segment].direction_rotate_z);
    }

    // Scaling
    if ((*motion_segments)[current_segment].scale)
    {
        motion.compute_incremental_scaling((*motion_segments)[current_segment].direction_scale);
    }
}

Motion* MotionPlan::get_motion()
{
    return &motion;
}

void MotionPlan::set_callback(void (*_reset_motion)(Motion*))
{
    reset_motion = _reset_motion;
}