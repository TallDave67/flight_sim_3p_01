#pragma once

#include <GL/glew.h>
#include <algorithm> // std::max

// Math
static const GLfloat DEGREES_IN_CIRCLE = 360.0f;
static const GLfloat DEGREES_IN_HALF_CIRCLE = 180.0f;
static const GLfloat DEGREES_IN_QUARTER_CIRCLE = 90.0f;

// Window Dimensions
static const GLint WINDOW_WIDTH = 800;
static const GLint WINDOW_HEIGHT = 600;

// Frame Rates
static const unsigned int FPS_WINDOW = 60;
static const unsigned int FPS_MP4 = 60;

// Mesh Vertices
static const GLint VERTEX_INFO_COORD_NUM = 3;
static const GLint VERTEX_INFO_TEXTURE_NUM = 2;
static const GLint VERTEX_INFO_NORMAL_NUM = 3;
static const GLint VERTEX_INFO_TOTAL_NUM = 8;

// Input Files
static const char* PATH_INPUT = "../../input/";
static const char* PATH_MODELS = "models/";
static const char* PATH_TEXTURES = "textures/";
static const char* PATH_SHADERS = "shaders/";
//
static const char* MODEL_EXTENSION = ".obj";
static const char* TEXTURE_DEFAULT = "plain.png";

// Motion
static const int AXIS_NONE = 0x00000000;
static const int AXIS_X = 0x00000001;
static const int AXIS_Y = 0x00000002;
static const int AXIS_Z = 0x00000004;
static const int DIRECTION_NEGATIVE = -1;
static const int DIRECTION_NONE = 0;
static const int DIRECTION_POSITIVE = 1;

// Translation
static const float TRANSLATION_OFFSET_START = 0.0f;
static const float TRANSLATION_MAX_OFFSET = 24.0f;
static const float TRANSLATION_INCREMENT = (1.0f / static_cast<float>(FPS_WINDOW));

// Rotation
static const float ROTATION_ANGLE_START = 0.0f;
static const float ROTATION_MAX_ANGLE = 360.0f;
static const float ROTATION_INCREMENT = (30.0f / static_cast<float>(FPS_WINDOW));

// Scaling
static const float SCALING_SIZE_START = 1.0f;
static const float SCALING_MIN_SIZE = 0.05f;
static const float SCALING_MAX_SIZE = 1.0f;
static const float SCALING_INCREMENT = (5.0f / static_cast<float>(FPS_WINDOW));

// Camera
static const float CAMERA_TRANSLATION_INCREMENT = 3.2f * TRANSLATION_INCREMENT;
static const float CAMERA_ROTATION_INCREMENT = 0.4f * ROTATION_INCREMENT;

// Lighting
static const unsigned int MAX_POINT_LIGHTS = 3;
static const unsigned int MAX_SPOT_LIGHTS = 3;
