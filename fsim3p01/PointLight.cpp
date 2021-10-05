#include "PointLight.h"


PointLight::PointLight() :
    position(0.0f, 0.0f, 0.0f), constant(1.0f), linear(0.0f), exponent(0.0f)
{
}

PointLight::~PointLight()
{
}

void PointLight::initialize(
    GLfloat red, GLfloat green, GLfloat blue,
    GLfloat _ambientIntensity, GLfloat _diffuseIntensity,
    GLfloat x, GLfloat y, GLfloat z,
    GLfloat _constant,  GLfloat _linear, GLfloat _exponent)
{
    Light::initialize(red, green, blue, _ambientIntensity, _diffuseIntensity);
    position = glm::vec3(x, y, z);
    constant = _constant;
    linear = _linear;
    exponent = _exponent;
}

void PointLight::UseLight(
    GLuint colourLocation, GLuint ambientIntensityLocation, GLuint diffuseIntensityLocation,
    GLuint positionLocation, GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
    Light::UseLight(colourLocation, ambientIntensityLocation, diffuseIntensityLocation);
    glUniform3f(positionLocation, position.x, position.y, position.z);
    glUniform1f(constantLocation, constant);
    glUniform1f(linearLocation, linear);
    glUniform1f(exponentLocation, exponent);
}

void PointLight::setPosition(glm::vec3 _position)
{
    position = _position;
}
