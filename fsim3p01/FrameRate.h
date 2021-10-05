#pragma once
#include <stdio.h>
#include <chrono>

class FrameRate
{
public:
    FrameRate(unsigned int fps);
    ~FrameRate();
    
    int get_frame_rate();
    bool is_frame_due();
    unsigned int get_next_frame();

private:
    unsigned int _fps;
    float _inter_frame_duration;
    unsigned int _next_frame;
    std::chrono::steady_clock::time_point _t0;
};

