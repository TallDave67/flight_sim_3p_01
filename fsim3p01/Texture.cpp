#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture() :
    textureID(0), width(0), height(0), bitDepth(0)
{
}

Texture::~Texture()
{
    ClearTexture();
}

void Texture::initialize(const char* _fileLocation)
{
    fileLocation = _fileLocation;
}

bool Texture::LoadTexture(GLenum format)
{
    // load the image data
    printf("loading texture %s\n", fileLocation.c_str());
    stbi_set_flip_vertically_on_load(true); 
    unsigned char* texData = stbi_load(fileLocation.c_str(), &width, &height, &bitDepth, 0);
    if (!texData)
    {
        printf("Failed to find %s\n", fileLocation.c_str());
        return false;
    }

    // let's generate a texture in the gl world
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // and set some display parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // how it will wrap along X axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // how it will wrap along Y axis
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // how we will minimize the image when we zoom out
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // how we will magnify the image when we zoom in

    // copy image data to our texture
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, texData);
    glGenerateMipmap(GL_TEXTURE_2D);

    // we are done talking to our gl texture
    glBindTexture(GL_TEXTURE_2D, 0);

    // free the memory associated with the loaded image because we have copied it to the gl texture
    stbi_image_free(texData);

    return true;
}

void Texture::UseTexture()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::ClearTexture()
{
    glDeleteTextures(1, &textureID);
    textureID = 0;
    width = 0;
    height = 0;
    bitDepth = 0;
    fileLocation = "";
}
