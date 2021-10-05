#include "FrameRate.h"

FrameRate::FrameRate(unsigned int fps) :
    _fps(fps), _next_frame(0)
{
    _inter_frame_duration = 1.0f / static_cast<float>(_fps);
}

FrameRate::~FrameRate()
{
}

int FrameRate::get_frame_rate()
{
    return _fps;
}

bool FrameRate::is_frame_due()
{
    bool is_due = false;

    // are we about to roll over our odo-framer?
    if (_next_frame == UINT64_MAX)
    {
        // how many frames have we completed for the current second?
        unsigned int frames_completed = _next_frame - (_next_frame / _fps);

        // how much of the current second have we framed?
        if (frames_completed == _fps)
        {
            // a rare occurrence to have completed all of the current second
            _next_frame = 0;
        }
        else
        {
             // get our new start time
            std::chrono::duration<float> duration(static_cast<float>(frames_completed) * _inter_frame_duration);
            _t0 = std::chrono::high_resolution_clock::now();

            // set our next frame
            _next_frame = frames_completed + 1;
        }
    }

    // ok, let's check if we are at a frame boundary
    if (_next_frame == 0)
    {
        _t0 = std::chrono::high_resolution_clock::now();
        _next_frame++;
    }
    else
    {
        // get time duration since stream start
        auto t1 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> duration = t1 - _t0;

        // have we hit the time boundary for the next frame?
        float _next_frame_time_boundary = static_cast<float>(_next_frame) * _inter_frame_duration;
        if (duration.count() > _next_frame_time_boundary)
        {
            is_due = true;
            //printf("stream frame = %d @ duration = %4.4f secs\n", _next_frame, duration.count());
            _next_frame++;
        }
    }

    return is_due;
}

unsigned int FrameRate::get_next_frame()
{
    return _next_frame;
}