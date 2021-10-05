#pragma once
#include <stdio.h>
#include <GL/glew.h>
#include "FrameRate.h"

class VideoStreamer
{
public:
    VideoStreamer(unsigned int fps);
    ~VideoStreamer();
    
    bool is_stream();

    void prepare(GLint width, GLint height);
    bool is_time_to_stream();
    void stream();
    void finish();

private:
    bool _is_stream;

    FrameRate _frame_rate;

    GLint _width;
    GLint _height;

    FILE* _ffmpeg;
    int* _buffer;
};

