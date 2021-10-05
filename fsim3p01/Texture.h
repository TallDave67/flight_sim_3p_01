#pragma once
#include <string>

#include <GL\glew.h>

class Texture
{
public:
    Texture();
    ~Texture();

    void initialize(const char* _fileLocation);

    bool LoadTexture(GLenum format);

    void UseTexture();
    void ClearTexture();

private:
    GLuint textureID;
    int width;
    int height;
    int bitDepth;

    std::string fileLocation;
};

