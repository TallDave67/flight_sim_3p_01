#include "Utility.h"

#include "Constants.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Utility::Utility()
{

}

Utility::~Utility()
{

}

void Utility::CalculateNormals(GLfloat* vertices, unsigned int num_vertices)
{
    //printf("CalculateNormals: ******************** \n");
    for (unsigned int i = 0; i < num_vertices; i += 3)
    {
        // grab the 3 vertices for the triangle face
        GLfloat x0 = vertices[i * VERTEX_INFO_TOTAL_NUM];
        GLfloat y0 = vertices[i * VERTEX_INFO_TOTAL_NUM + 1];
        GLfloat z0 = vertices[i * VERTEX_INFO_TOTAL_NUM + 2];
        GLfloat x1 = vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM];
        GLfloat y1 = vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM + 1];
        GLfloat z1 = vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM + 2];
        GLfloat x2 = vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM];
        GLfloat y2 = vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM + 1];
        GLfloat z2 = vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM + 2];

        // create vectors for 2 of the triangle's sides
        glm::vec3 s = glm::vec3(x1, y1, z1) - glm::vec3(x0, y0, z0);
        glm::vec3 t = glm::vec3(x2, y2, z2) - glm::vec3(x0, y0, z0);

        // compute the normal
        glm::vec3 n = glm::normalize(glm::cross(s, t));
        //printf("i = %u, v0 = %2.4f, %2.4f, %2.4f, v1 = %2.4f, %2.4f, %2.4f, v2 = %2.4f, %2.4f, %2.4f, n = %2.4f, %2.4f, %2.4f\n", 
        //    i, x0, y0, z0, x1, y1, z1, x2, y2, z2, n.x, n.y, n.z);

        // give each of the 3 vertices this same normal
        unsigned int normal_offset = VERTEX_INFO_COORD_NUM + VERTEX_INFO_TEXTURE_NUM;
        vertices[i * VERTEX_INFO_TOTAL_NUM + normal_offset] = n.x;
        vertices[i * VERTEX_INFO_TOTAL_NUM + normal_offset + 1] = n.y;
        vertices[i * VERTEX_INFO_TOTAL_NUM + normal_offset + 2] = n.z;
        vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM + normal_offset] = n.x;
        vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM + normal_offset + 1] = n.y;
        vertices[(i + 1) * VERTEX_INFO_TOTAL_NUM + normal_offset + 2] = n.z;
        vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM + normal_offset] = n.x;
        vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM + normal_offset + 1] = n.y;
        vertices[(i + 2) * VERTEX_INFO_TOTAL_NUM + normal_offset + 2] = n.z;
    }
}

void Utility::debugFragmentShader_ApplyLight()
{
    glm::vec3 fragPos(0.0f, 0.0f, 0.0f);
    glm::vec3 light(0.0f, 0.0f, -1.0f);
    glm::vec3 normal(0.0f, 0.0f, 1.0f);
    glm::vec3 eye(0.0f, 0.0f, 1.0f);

    glm::vec3 colour(1.0f, 1.0f, 1.0f);
    GLfloat ambientIntensity = 0.2f;
    GLfloat diffuseIntensity = 0.2f;
    GLfloat specularIntensity = 1.0f;
    GLfloat shininess = 32.0f;
        
    glm::vec4 ambientColour = glm::vec4(colour, 1.0f) * ambientIntensity;

    GLfloat diffuseFactor = glm::max(glm::dot(glm::normalize(normal), -glm::normalize(light)), 0.0f);
    glm::vec4 diffuseColour = glm::vec4(colour, 1.0f) * diffuseIntensity * diffuseFactor;

    glm::vec3 fragToEye = glm::normalize(eye - fragPos);
    glm::vec3 reflection = glm::normalize(glm::reflect(glm::normalize(light), glm::normalize(normal)));

    GLfloat specularFactor = 0.0f;
    glm::vec4 specularColour = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if (diffuseFactor > 0.0f)
    {
        specularFactor = dot(fragToEye, reflection);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, shininess);
            specularColour = glm::vec4(light, 1.0f) * specularIntensity * specularFactor;
        }
    }

    printf("ambientColour: x=%2.6f, y=%2.6f, z=%2.6f, a=%2.6f\n\
        diffuseFactor=%2.6f;diffuseColour: x=%2.6f, y=%2.6f, z=%2.6f, a=%2.6f\n\
        fragToEye: x=%2.6f, y=%2.6f, z=%2.6f\n\
        reflection: x=%2.6f, y=%2.6f, z=%2.6f\n\
        specularFactor=%2.6f;specularColour: x=%2.6f, y=%2.6f, z=%2.6f, a=%2.6f\n",
        ambientColour.x, ambientColour.y, ambientColour.z, ambientColour.a,
        diffuseFactor, diffuseColour.x, diffuseColour.y, diffuseColour.z, diffuseColour.a,
        fragToEye.x, fragToEye.y, fragToEye.z,
        reflection.x, reflection.y, reflection.z,
        specularFactor, specularColour.x, specularColour.y, specularColour.z, specularColour.a);
}

void Utility::axis_angle_to_euler(float x, float y, float z, float angle, float & heading, float & attitude, float & bank) 
{
	float s=sin(angle);
	float c=cos(angle);
	float t=1-c;
	// if axis is not already normalised then uncomment this
	// double magnitude = Math.sqrt(x*x + y*y + z*z);
	// if (magnitude==0) throw error;
	// x /= magnitude;
	// y /= magnitude;
	// z /= magnitude;
	if ((x*y*t + z*s) > 0.998f) { // north pole singularity detected
		heading = 2*atan2(x*sin(angle/2),cos(angle/2));
		attitude = static_cast<float>(M_PI)/2.0f;
		bank = 0;
		return;
	}
	if ((x*y*t + z*s) < -0.998f) { // south pole singularity detected
		heading = -2*atan2(x*sin(angle/2),cos(angle/2));
		attitude = -static_cast<float>(M_PI)/2.0f;
		bank = 0;
		return;
	}
	heading = atan2(y * s- x * z * t , 1 - (y*y+ z*z ) * t);
	attitude = asin(x * y * t + z * s) ;
	bank = atan2(x * s - y * z * t , 1 - (x*x + z*z) * t);
}