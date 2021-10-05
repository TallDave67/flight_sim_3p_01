#version 460

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 colour;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;

uniform vec3 eye;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    // We need to calculate the angle between the vertex’s normal vector and a vector pointing at the light source from the vertex.
    // So we reverse the sign on the light direction vector which has its origin at the light source.
    float diffuseFactor = max(dot(normalize(Normal), -normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(light.ambientIntensity > 0 && light.diffuseIntensity > 0.0f && diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eye - FragPos);
        vec3 reflection = normalize(reflect(normalize(direction), normalize(Normal)));
        float specularFactor = dot(fragToEye,reflection);
        if(specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour, 1.0f) * material.specularIntensity * specularFactor;
        }
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLight(PointLight pointLight)
{
    // calculate direction
    vec3 direction = FragPos - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    // and now the color
    vec4 colour = CalcLightByDirection(pointLight.base, direction);

    // but now we must attenuate the light
    float attenuation = pointLight.exponent * (distance * distance) + 
                        pointLight.linear * distance + 
                        pointLight.constant;
    return (colour / attenuation);
}

vec4 CalcSpotLight(SpotLight spotLight)
{
    // calculate the direction
    vec3 rayDirection = normalize(FragPos - spotLight.base.position);

    // and now the angle at which the the spotlight is hitting the fragment
    float slFactor = max(dot(normalize(rayDirection), normalize(spotLight.direction)), 0.0f);

    // are we in the cone of the spotlight
    vec4 colour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(slFactor > 0.0f && slFactor > spotLight.edge)
    {
        // map slFactor to a 0.0 to 1.0 scale
        float ratio = 1.0f / (1.0f - spotLight.edge);
        float diff = (1.0f - slFactor) * ratio;
        slFactor = 1.0f - diff; 

        // calculate the point light portion
        colour = CalcPointLight(spotLight.base);
    }

    return colour;
}

vec4 CalcPointlLights()
{
   vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
   for(int i = 0; i < pointLightCount; i++)
   {
      totalColour += CalcPointLight(pointLights[i]);
   }
   return totalColour;
}

vec4 CalcSpotlLights()
{
   vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);
   for(int i = 0; i < spotLightCount; i++)
   {
      totalColour += CalcSpotLight(spotLights[i]);
   }
   return totalColour;
}

void main()
{
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointlLights();
    finalColour += CalcSpotlLights();
    colour = texture(theTexture, TexCoord) * finalColour;
}
