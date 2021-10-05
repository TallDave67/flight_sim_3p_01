#include <stdio.h>
#include <string.h>
#include <memory>
#include <vector>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Constants.h"
#include "DrawingWindow.h"
#include "Model.h"
#include "Mesh.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Shader.h"
#include "Motion.h"
#include "MotionPlan.h"
#include "MotionSegment.h"
#include "Camera.h"

#include "FrameRate.h"
#include "VideoStreamer.h"

// Models
Model flyer;
std::string model_flyer = std::string("Viper-mk-IV-fighter");
Model floater;
std::string model_floater = std::string("UHFSatcom");

// Meshes
std::vector<std::shared_ptr<Mesh>> meshList;

// Ground Mesh
GLfloat ground_vertices[] = {
    //   x       y      z        u       v    nx    ny    nz
    -12.0f,  -6.0f,  12.0f,  0.00f,  0.00f, 0.0f, 1.0f, 0.0f, //corner bottom left
     12.0f,  -6.0f,  12.0f,  0.00f,  1.00f, 0.0f, 1.0f, 0.0f, //corner bottom right
     12.0f,  -6.0f, -12.0f,  1.00f,  1.00f, 0.0f, 1.0f, 0.0f, //corner top right
    -12.0f,  -6.0f, -12.0f,  1.00f,  0.00f, 0.0f, 1.0f, 0.0f, //corner top left
};
unsigned int num_ground_vertices = 32;
unsigned int ground_indices[] = {
    0, 2, 3,
    0, 1, 2
};
unsigned int num_ground_indices = 6;

// Textures
Texture groundTexture;
std::string texture_ground_path = std::string(PATH_INPUT) + std::string(PATH_TEXTURES) + std::string("ground.png");

void CreateObjects()
{
    // Create Objects with Models
    flyer.initialize(model_flyer.c_str());
    flyer.LoadModel();
    floater.initialize(model_floater.c_str());
    floater.LoadModel();

    // Create Objects without Models
    meshList.push_back(std::make_shared<Mesh>());
    meshList[0]->CreateMesh(ground_vertices, ground_indices, num_ground_vertices, num_ground_indices);
    groundTexture.initialize(texture_ground_path.c_str());
    groundTexture.LoadTexture(GL_RGBA);
}

// Lighting
DirectionalLight directionalLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Material
Material shinyMaterial;
Material dullMaterial;

// Shaders
std::string vertex_code_path =  std::string(PATH_INPUT) + std::string(PATH_SHADERS) + std::string("shader.vert");
std::string fragment_code_path =  std::string(PATH_INPUT) + std::string(PATH_SHADERS) + std::string("shader.frag");
std::vector<std::shared_ptr<Shader>> shaderList;

void CreateShaders()
{
    shaderList.push_back(std::make_shared<Shader>());
    shaderList[0]->CreateFromFile(vertex_code_path.c_str() , fragment_code_path.c_str());
}

// Motion
std::vector<MotionSegment> flyerMotionSegments {
    {   11 * FPS_WINDOW, 0, 
        true, DIRECTION_NEGATIVE, DIRECTION_NONE, DIRECTION_POSITIVE,
        false, {0.0f, 0.0f, 0.0f}, DIRECTION_NONE, 
        false, DIRECTION_NONE
    },
    {   5 * FPS_WINDOW, 0, 
        true, DIRECTION_NEGATIVE, DIRECTION_NONE, DIRECTION_NONE,
        true, {0.0f, 1.0f, 0.0f}, DIRECTION_NEGATIVE, 
        false, DIRECTION_NONE
    },
    {   11 * FPS_WINDOW, 0, 
        true, DIRECTION_NONE, DIRECTION_NONE, DIRECTION_NEGATIVE,
        false, {0.0f, 0.0f, 0.0f}, DIRECTION_NONE, 
        false, DIRECTION_NONE
    },
    {   5 * FPS_WINDOW, 0, 
        true, DIRECTION_POSITIVE, DIRECTION_NONE, DIRECTION_NONE,
        true, {0.0f, 1.0f, 0.0f}, DIRECTION_NEGATIVE, 
        false, DIRECTION_NONE
    },
    {   8 * FPS_WINDOW, 0, 
        true, DIRECTION_POSITIVE, DIRECTION_POSITIVE, DIRECTION_POSITIVE,
        false, {0.0f, 0.0f, 0.0f}, DIRECTION_NONE, 
        false, DIRECTION_NONE
    }
};

// Camera
Camera camera;

int main()
{
    // Our main drawing window
    DrawingWindow mainWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
    int ret = mainWindow.initialize();
    if (ret != 0)
    {
        return ret;
    }

    // Objects to draw and how to draw them
    CreateObjects();
    CreateShaders();

    // Initiliaze our camera
    camera.initialize(&mainWindow, glm::vec3(0.0f, 0.0f, TRANSLATION_MAX_OFFSET), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 
        static_cast<GLfloat>(CAMERA_TRANSLATION_INCREMENT), static_cast<GLfloat>(CAMERA_ROTATION_INCREMENT));

    // Lighting
    directionalLight.initialize(
        1.0f, 1.0f, 1.0f, 
        0.3f, 0.1f, 
        0.0f, 0.0f, -1.0f
    );
    //
    unsigned int pointLightCount = 0;
    pointLights[0].initialize(
        0.0f, 0.0f, 1.0f,
        0.6f, 0.1f,
        0.0f, 1.1f, -5.0f,
        0.3f, 0.1f, 0.1f
    );
    pointLightCount++;
    pointLights[1].initialize(
        0.0f, 0.0f, 1.0f,
        0.7f, 1.0f,
        -4.0f, 2.0f, 4.0f,
        0.3f, 0.1f, 0.1f
    );
    //pointLightCount++;
    pointLights[2].initialize(
        0.0f, 0.0f, 1.0f,
        0.7f, 1.0f,
        0.0f, 8.0f, -4.0f,
        0.3f, 0.1f, 0.1f
    );
    pointLightCount++;
    
    unsigned int spotLightCount = 0;
    spotLights[0].initialize(
        1.0f, 1.0f, 1.0f,
        2.0f, 0.2f,
        -4.0f, 0.5f, 3.0f,
        0.3f, 0.2f, 0.1f,
        0.0f, -1.0f, 0.0f, 0.71f
    );
    spotLightCount++;
    spotLights[1].initialize(
        1.0f, 1.0f, 1.0f,
        2.0f, 0.2f,
        -4.0f, 0.5f, 3.0f,
        0.3f, 0.2f, 0.1f,
        0.0f, 1.0f, 0.0f, 0.71f
    );
    spotLightCount++;

    // Material
    shinyMaterial.initialize(1.0f, 32.0f);
    dullMaterial.initialize(0.2f, 4.0f);

    // Create a perspective projection so we can live in a 3d world
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), static_cast<GLfloat>(mainWindow.getBufferWidth())/static_cast<GLfloat>(mainWindow.getBufferHeight()), 0.1f, 150.0f);

    // Set a reasonable frame rate
    FrameRate fr(FPS_WINDOW);

    // Video Streaming
    VideoStreamer video_streamer(FPS_MP4);
    video_streamer.prepare(mainWindow.getWidth(), mainWindow.getHeight());

    // Motion
    MotionPlan flyerMotionPlan;
    flyerMotionPlan.initialize(
        glm::vec3(29.5f, 0.0f, -17.5f), glm::vec3(0.0f, 1.0f, 0.0f), -30.0f, 1.0f,
        2.5f, 2.5f, 2.5f, 1.0f, 1.0f, &flyerMotionSegments, true);
    Motion* flyerMotion = flyerMotionPlan.get_motion();
    //
    Motion floaterMotion;
    floaterMotion.initialize(
        glm::vec3(-2.3f, 1.0f, -4.7f), glm::vec3(0.0f, 1.0f, 1.0f), 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.5f, 1.0f);
    //
    Motion spotlightMotion;
    spotlightMotion.initialize(
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 2.0f, 1.0f);

    // Loop until window closed
    while (!mainWindow.shouldClose())
    {
        // Do we render to the screen?
        if (!fr.is_frame_due())
        {
            continue;
        }

        // Get + Handle user input events
        glfwPollEvents();
        camera.handleKeyEvent();

        // Motion
        flyerMotionPlan.move();
        floaterMotion.rotate(glm::vec3(0.0f, 1.0f, 1.0f), DIRECTION_POSITIVE);
        spotlightMotion.rotate(glm::vec3(0.0f, 1.0f, 0.0f), DIRECTION_POSITIVE);

        // Clear window
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderList[0]->UseShader();
    
        // Light
        shaderList[0]->SetDirectionalLight(&directionalLight);
        shaderList[0]->SetPointLights(pointLights, pointLightCount);
        //
        spotLights[0].setDirection(spotlightMotion.getDirection(glm::vec3(0.0f, 0.0f, 0.0001f)));
        spotLights[1].setDirection(-spotlightMotion.getDirection(glm::vec3(0.0f, 0.0f, 0.0001f)));
        shaderList[0]->SetSpotLights(spotLights, spotLightCount);

        // Projection
        glUniformMatrix4fv(shaderList[0]->GetProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projection));

        // Camera (View)
        glUniformMatrix4fv(shaderList[0]->GetViewLocation(), 1, GL_FALSE, glm::value_ptr(camera.getView()));

        // Eye
        glUniform3f(shaderList[0]->GetEyeLocation(), camera.getCameraEye().x, camera.getCameraEye().y, camera.getCameraEye().z);
        
        // Flyer Model
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(flyerMotion->getX(), flyerMotion->getY(), flyerMotion->getZ()));
        model = glm::rotate(model, glm::radians(80.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(flyerMotion->getAngle()), flyerMotion->getAxisOfRotation());
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
        flyer.RenderModel();

        // Floater Model
        model = glm::mat4 (1.0f);
        model = glm::translate(model, glm::vec3(-2.3f, 1.0f, -4.7f));
        model = glm::rotate(model, glm::radians(floaterMotion.getAngle()), floaterMotion.getAxisOfRotation());
        model = glm::scale(model, glm::vec3(0.75f, 0.75f, 0.75f));
        glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));
        shinyMaterial.UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
        floater.RenderModel();

        // Model with ground mesh
        model = glm::mat4 (1.0f);
        glUniformMatrix4fv(shaderList[0]->GetModelLocation(), 1, GL_FALSE, glm::value_ptr(model));;
        groundTexture.UseTexture();
        dullMaterial.UseMaterial(shaderList[0]->GetSpecularIntensityLocation(), shaderList[0]->GetShininessLocation());
        meshList[0]->RenderMesh();

        glUseProgram(0);

        // stream before rendering
        video_streamer.stream();

        // render
        mainWindow.swapBuffers();
    }

    // finish streaming
    video_streamer.finish();

    // report the number of frames rendered
    printf("rendered %u frames\n", fr.get_next_frame() - 1);

    return 0;
}