#include "Material.h"

Material::Material() :
    specularIntensity(0.0f), shininess(0.0f)
{
}

Material::~Material()
{
}

void Material::initialize(GLfloat _specularIntensity, GLfloat _shininess)
{
    specularIntensity = _specularIntensity;
    shininess = _shininess;
}

void Material::UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation)
{
    glUniform1f(specularIntensityLocation, specularIntensity);
    glUniform1f(shininessLocation, shininess);
}
