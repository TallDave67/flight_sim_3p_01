#include "MotionSegment.h"

#include "Constants.h"

MotionSegment::MotionSegment(
        unsigned int _num_frames, unsigned int _current_frame,
        bool _translate, int _direction_x, int _direction_y, int _direction_z,
        bool _rotate, int _axes_of_rotation, int _direction_rotate,
        bool _scale, int _direction_scale) :
    num_frames(_num_frames), current_frame(_current_frame),
    translate(_translate), direction_x(_direction_x), direction_y(_direction_y), direction_z(_direction_z),
    rotate(_rotate), axes_of_rotation(_axes_of_rotation), direction_rotate(_direction_rotate),
    scale(_scale), direction_scale(_direction_scale)
{
}

MotionSegment::~MotionSegment()
{
}

unsigned int MotionSegment::reset()
{
    current_frame = 0;
    return num_frames;
}

bool MotionSegment::next()
{
    current_frame++;
    return current_frame <= num_frames;
}

void MotionSegment::execute()
{
}
