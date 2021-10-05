#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :
    direction(0.0f, -1.0f, 0.0f)
{
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::initialize(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
    GLfloat x, GLfloat y, GLfloat z)
{
    Light::initialize(red, green, blue, _ambientIntensity, _diffuseIntensity);
    direction = glm::vec3(x, y, z);
}

void DirectionalLight::UseLight(
    GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
    GLuint directionLocation)
{
    Light::UseLight(colourLocation, ambientIntensityLocation, diffuseIntensityLocation);
    glUniform3f(directionLocation, direction.x, direction.y, direction.z);
}