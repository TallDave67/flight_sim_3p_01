#include "VideoStreamer.h"
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

const int cmd_len = 1024;
const char* cmd_fmt = "ffmpeg -r %u -f rawvideo -pix_fmt rgba -s %dx%d -i - -threads 0 -preset fast -y -pix_fmt yuv420p -crf 21 -vf vflip output.mp4";

VideoStreamer::VideoStreamer(unsigned int fps) :
    _is_stream(true),  _frame_rate(fps), _width(0), _height(0), _ffmpeg(NULL), _buffer(NULL)
{
}

VideoStreamer::~VideoStreamer()
{
    // finish any streaming in progress
    finish();
}

bool VideoStreamer::is_stream()
{
    return _is_stream;
}

void VideoStreamer::prepare(GLint width, GLint height)
{
    // are we supposed to stream?
    if (!_is_stream)
    {
        // no
        return;
    }

    // finish any streaming in progress
    finish();
    
    // do we have a good width and height?
    if (width > 0 && height > 0)
    {
        _width = width;
        _height = height;

        // create our ffmpeg command
        char command[cmd_len];
        sprintf_s(command, cmd_len, cmd_fmt, _frame_rate.get_frame_rate(), width, height);
 
        // open pipe to ffmpeg's stdin in binary write mode
        _ffmpeg = _popen(command, "wb");

        // create a buffer to hold the framebuffer data
        _buffer = (int *) malloc(sizeof(int) * _width * _height);

    }
}

bool VideoStreamer::is_time_to_stream()
{
    return _frame_rate.is_frame_due();
}

void VideoStreamer::stream()
{
    // are we supposed to stream?
    if (!_is_stream)
    {
        // no
        return;
    }

    if (is_time_to_stream())
    {
        // Grab the framebuffer and send it straight to the encoder
        glReadPixels(0, 0, _width, _height, GL_RGBA, GL_UNSIGNED_BYTE, _buffer);
        fwrite(_buffer, sizeof(int)*_width*_height, 1, _ffmpeg);
    }
}

void VideoStreamer::finish()
{
    // are we supposed to stream?
    if (!_is_stream)
    {
        // no
        return;
    }

    // close the video file
    if (_ffmpeg)
    {
        _pclose(_ffmpeg);
        _ffmpeg = NULL;
    }

    // release memory for each frame
    if (_buffer)
    {
        free(_buffer);
        _buffer = NULL;
    }
}

