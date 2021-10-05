#include "Shader.h"

Shader::Shader() :
    shaderID(0), uniformProjection(0), uniformView(0), uniformEye(0), uniformModel(0),
    uniformSpecularIntensity(0), uniformShininess(0), uniformPointLightCount(0), uniformSpotLightCount(0)
{
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode)
{
    CompileShader(vertexCode, fragmentCode);
}

void Shader::CreateFromFile(const char* vertexCodePath, const char* fragmentCodePath)
{
    std::string vertexCodeString = ReadFile(vertexCodePath);
    std::string fragmentCodeString = ReadFile(fragmentCodePath);
    CreateFromString(vertexCodeString.c_str(), fragmentCodeString.c_str());
}

GLuint Shader::GetProjectionLocation()
{
    return uniformProjection;
}

GLuint Shader::GetViewLocation()
{
    return uniformView;
}

GLuint Shader::GetEyeLocation()
{
    return uniformEye;
}

GLuint Shader::GetModelLocation()
{
    return uniformModel;
}

GLuint Shader::GetSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetColourLocation()
{
    return uniformDirectionalLight.uniformColour;
}

GLuint Shader::GetAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight)
{
    directionalLight->UseLight(
        uniformDirectionalLight.uniformColour,
        uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformDiffuseIntensity,
        uniformDirectionalLight.uniformDirection);
}

void Shader::SetPointLights(PointLight* _pointLights, unsigned int _pointLightCount)
{
    if (_pointLightCount > MAX_POINT_LIGHTS)
    {
        _pointLightCount = MAX_POINT_LIGHTS;
    }

    glUniform1i(uniformPointLightCount, _pointLightCount);

    for (unsigned int i = 0; i < _pointLightCount; i++)
    {
        _pointLights[i].UseLight(
            uniformPointLights[i].uniformColour,
            uniformPointLights[i].uniformAmbientIntensity, uniformPointLights[i].uniformDiffuseIntensity,
            uniformPointLights[i].uniformPosition, uniformPointLights[i].uniformConstant,
            uniformPointLights[i].uniformLinear, uniformPointLights[i].uniformExponent
        );
    }
}

void Shader::SetSpotLights(SpotLight* _spotLights, unsigned int _spotLightCount)
{
    if (_spotLightCount > MAX_SPOT_LIGHTS)
    {
        _spotLightCount = MAX_SPOT_LIGHTS;
    }

    glUniform1i(uniformSpotLightCount, _spotLightCount);

    for (unsigned int i = 0; i < _spotLightCount; i++)
    {
        _spotLights[i].UseLight(
            uniformSpotLights[i].uniformColour,
            uniformSpotLights[i].uniformAmbientIntensity, uniformSpotLights[i].uniformDiffuseIntensity,
            uniformSpotLights[i].uniformPosition, uniformSpotLights[i].uniformConstant,
            uniformSpotLights[i].uniformLinear, uniformSpotLights[i].uniformExponent,
            uniformSpotLights[i].uniformDirection, uniformSpotLights[i].uniformEdge
        );
    }
}

void Shader::UseShader()
{
    if (shaderID)
    {
        glUseProgram(shaderID);
    }
}

void Shader::ClearShader()
{
    if (shaderID)
    {
        glDeleteProgram(shaderID);
        shaderID = 0;
    }

    uniformProjection = 0;
    uniformView = 0;
    uniformEye = 0;
    uniformModel = 0;
    uniformSpecularIntensity = 0;
    uniformShininess = 0;

    uniformDirectionalLight.uniformColour = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;

    uniformPointLightCount = 0;
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        uniformPointLights[i].uniformColour = 0;
        uniformPointLights[i].uniformAmbientIntensity = 0;
        uniformPointLights[i].uniformDiffuseIntensity = 0;
        uniformPointLights[i].uniformPosition = 0;
        uniformPointLights[i].uniformConstant = 0;
        uniformPointLights[i].uniformLinear = 0;
        uniformPointLights[i].uniformExponent = 0;
    }

    uniformSpotLightCount = 0;
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        uniformSpotLights[i].uniformColour = 0;
        uniformSpotLights[i].uniformAmbientIntensity = 0;
        uniformSpotLights[i].uniformDiffuseIntensity = 0;
        uniformSpotLights[i].uniformPosition = 0;
        uniformSpotLights[i].uniformConstant = 0;
        uniformSpotLights[i].uniformLinear = 0;
        uniformSpotLights[i].uniformExponent = 0;
        uniformSpotLights[i].uniformDirection = 0;
        uniformSpotLights[i].uniformEdge = 0;
    }
}

std::string Shader::ReadFile(const char* path)
{
    std::string content;
    std::ifstream filestream(path, std::ios::in);

    if (!filestream.is_open())
    {
        printf("Failed to read %s!  File doesn't exist", path);
    }
    else
    {
        std::string line;
        while (!filestream.eof())
        {
            std::getline(filestream, line);
            content += line + '\n';
        }
        filestream.close();
    }
    return content;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program!\n");
        return;
    }

    AddShader(shaderID, vertexCode, GL_VERTEX_SHADER);
    AddShader(shaderID, fragmentCode, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error linking program: '%s'\n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Error validating program: '%s'\n", eLog);
        return;
    }

    uniformProjection = glGetUniformLocation(shaderID, "projection");
    uniformView = glGetUniformLocation(shaderID, "view");
    uniformEye = glGetUniformLocation(shaderID, "eye");
    uniformModel = glGetUniformLocation(shaderID, "model");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");
    for (unsigned int i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[128] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].base.colour", i);
        uniformPointLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].base.ambientIntensity", i);
        uniformPointLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].base.diffuseIntensity", i);
        uniformPointLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].position", i);
        uniformPointLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].constant", i);
        uniformPointLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].linear", i);
        uniformPointLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%u].exponent", i);
        uniformPointLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);
    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");
    for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[128] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.base.colour", i);
        uniformSpotLights[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.base.ambientIntensity", i);
        uniformSpotLights[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.base.diffuseIntensity", i);
        uniformSpotLights[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.position", i);
        uniformSpotLights[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.constant", i);
        uniformSpotLights[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.linear", i);
        uniformSpotLights[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].base.exponent", i);
        uniformSpotLights[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].direction", i);
        uniformSpotLights[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%u].edge", i);
        uniformSpotLights[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);
    }
}

void Shader::AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderCode;

    GLint codeLength[1];
    codeLength[0] = static_cast<GLint>(strlen(shaderCode));

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling the %d shader: '%s'\n", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
}




