#include "Light.h"

#include <GL/glew.h>
#include <glm/glm.hpp>

Light::Light() :
    colour(1.0f, 1.0f, 1.0f), ambientIntensity(1.0),  diffuseIntensity(0.0f)
{
}

Light::~Light()
{
}

void Light::initialize(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat _ambientIntensity, GLfloat _diffuseIntensity)
{
    colour = glm::vec3(red, green, blue);
    ambientIntensity = _ambientIntensity;
    diffuseIntensity = _diffuseIntensity;
    //printf("Light::initialize() red = %2.4f, green = %2.4f, blue = %2.4f, _ambientIntensity = %2.4f,  _diffuseIntensity = %2.4f\n", 
    //    red, green, blue, _ambientIntensity, _diffuseIntensity);

}

void Light::UseLight(GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation)
{
    //printf("Light::UseLight() red = %2.4f, green = %2.4f, blue = %2.4f, _ambientIntensity = %2.4f,  _diffuseIntensity = %2.4f\n", 
    //    colour.x, colour.y, colour.z, ambientIntensity, diffuseIntensity);
    glUniform3f(colourLocation, colour.x, colour.y, colour.z);
    glUniform1f(ambientIntensityLocation, ambientIntensity);
    glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
