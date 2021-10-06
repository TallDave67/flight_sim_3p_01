#include "MotionSegment.h"

#include "Constants.h"

MotionSegment::MotionSegment(
        unsigned int _num_frames, unsigned int _current_frame,
        bool _translate, int _direction_x, int _direction_y, int _direction_z,
        bool _rotate, int _direction_rotate_x, int _direction_rotate_y, int _direction_rotate_z,
        bool _scale, int _direction_scale) :
    num_frames(_num_frames), current_frame(_current_frame),
    translate(_translate), direction_x(_direction_x), direction_y(_direction_y), direction_z(_direction_z),
    rotate(_rotate), direction_rotate_x(_direction_rotate_x), direction_rotate_y(_direction_rotate_y), direction_rotate_z(_direction_rotate_z),
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
