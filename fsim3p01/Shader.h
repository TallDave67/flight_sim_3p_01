#pragma once

#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include <GL/glew.h>

#include "Constants.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

struct UNIFORM_LIGHT
{
    GLuint uniformColour;
    GLuint uniformAmbientIntensity;
    GLuint uniformDiffuseIntensity;

    UNIFORM_LIGHT()
    {
        uniformColour = 0;
        uniformAmbientIntensity = 0;
        uniformDiffuseIntensity = 0;
    }

    UNIFORM_LIGHT(GLuint c, GLuint ai, GLuint di)
    {
        uniformColour = c;
        uniformAmbientIntensity = ai;
        uniformDiffuseIntensity = di;
    }
};

struct UNIFORM_DIRECTIONAL_LIGHT : UNIFORM_LIGHT 
{
    GLuint uniformDirection;

    UNIFORM_DIRECTIONAL_LIGHT()
    {
        uniformDirection = 0;
    }

    UNIFORM_DIRECTIONAL_LIGHT(GLuint c, GLuint ai, GLuint di, GLuint d) :
        UNIFORM_LIGHT(c, ai, di)
    {
        uniformDirection = d;
    }
};

struct UNIFORM_POINT_LIGHT : UNIFORM_LIGHT 
{
    GLuint uniformPosition;
    GLuint uniformConstant;
    GLuint uniformLinear;
    GLuint uniformExponent;

    UNIFORM_POINT_LIGHT()
    {
        uniformPosition = 0;
        uniformConstant = 0;
        uniformLinear = 0;
        uniformExponent = 0;
    }

    UNIFORM_POINT_LIGHT(GLuint c, GLuint ai, GLuint di, GLuint p, GLuint co, GLuint l, GLuint e) :
        UNIFORM_LIGHT(c, ai, di)
    {
        uniformPosition = p;
        uniformConstant = co;
        uniformLinear = l;
        uniformExponent = e;
    }
};

struct UNIFORM_SPOT_LIGHT : UNIFORM_POINT_LIGHT 
{
    GLuint uniformDirection;
    GLuint uniformEdge;

    UNIFORM_SPOT_LIGHT()
    {
        uniformDirection = 0;
        uniformEdge = 0;
    }

    UNIFORM_SPOT_LIGHT(GLuint c, GLuint ai, GLuint di, GLuint p, GLuint co, GLuint l, GLuint e, GLuint d, GLuint ed) :
        UNIFORM_POINT_LIGHT(c, ai, di, p, co, l, e)
    {
        uniformDirection = d;
        uniformEdge = ed;
    }
};

class Shader
{
public:
    Shader();
    ~Shader();

    void CreateFromString(const char* vertexCode, const char* fragmentCode);
    void CreateFromFile(const char* vertexCodePath, const char* fragmentCodePath);

    GLuint GetProjectionLocation();
    GLuint GetViewLocation();
    GLuint GetEyeLocation();
    GLuint GetModelLocation();
    GLuint GetSpecularIntensityLocation();
    GLuint GetShininessLocation();

    GLuint GetColourLocation();
    GLuint GetAmbientIntensityLocation();
    GLuint GetDiffuseIntensityLocation();
    GLuint GetDirectionLocation();

    void SetDirectionalLight(DirectionalLight* directionalLight);
    void SetPointLights(PointLight* _pointLights, unsigned int _pointLightCount);
    void SetSpotLights(SpotLight* _spotLights, unsigned int _spotLightCount);

    void UseShader();
    void ClearShader();

private:
    GLuint shaderID;
    GLuint uniformProjection;
    GLuint uniformView;
    GLuint uniformEye;
    GLuint uniformModel;
    GLuint uniformSpecularIntensity;
    GLuint uniformShininess;

    UNIFORM_DIRECTIONAL_LIGHT uniformDirectionalLight;

    GLuint uniformPointLightCount;
    UNIFORM_POINT_LIGHT uniformPointLights[MAX_POINT_LIGHTS];

    GLuint uniformSpotLightCount;
    UNIFORM_SPOT_LIGHT uniformSpotLights[MAX_SPOT_LIGHTS];

private:
    std::string ReadFile(const char* path);
    void CompileShader(const char* vertexCode, const char* fragmentCode);
    void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType);
};

