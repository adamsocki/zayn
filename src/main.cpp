


#define OPENGL 0
#define VULKAN 1

#ifdef VULKAN
#define ENGINE_2D 0
#define ENGINE_3D 1
#endif


#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>
#include <unistd.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "../include/tiny_obj_loader.h"



#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <chrono>


#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
// #include <glm/glm.hpp>

#include "data_types.h"

#include "config.h"


#if WINDOWS
#include <cstring>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>


    



struct SystemPlatform
{
    bool running;
};



void framebuffer_size_callback(GLFWwindow* window, int width, int height);



#include "zayn.cpp"


int main(void)
{


    SystemPlatform platform = {};
    ZaynPlatform zaynPlatform = {};

    ZaynMemory* zaynMemory = &zaynPlatform.zaynMemory;

    zaynPlatform.running = true;

    Zayn = zaynMemory;

    AllocateMemoryArena(&Zayn->permanentMemArena, Megabytes(256));
    AllocateMemoryArena(&Zayn->frameMemArena, Megabytes(32));
    // InitInput();
    InputManager *inputManager = &zaynMemory->inputManager;


    AllocateInputManager(inputManager, &zaynMemory->permanentMemArena, 4);

    zaynMemory->keyboard = &inputManager->devices[0];
    AllocateInputDevice(zaynMemory->keyboard, InputDeviceType_Keyboard, Input_KeyboardDiscreteCount, 0);

    Keyboard = zaynMemory->keyboard;
    InitializeKeyMap();

    /* Initialize the library */
    if (!glfwInit())
    {

        return -1;
    }

#if OPENGL
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#elif VULKAN   
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
#endif

#ifdef __APPLE__
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


#endif
   /* Create a windowed mode window and its OpenGL context */


    Zayn->windowSize.x = 1000;
    Zayn->windowSize.y = 800;

    Zayn->window = glfwCreateWindow(Zayn->windowSize.x, Zayn->windowSize.y, "Hello World", NULL, NULL);
    if (!Zayn->window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(Zayn->window);
    glfwSetWindowUserPointer(Zayn->window, Zayn);
    glfwSetFramebufferSizeCallback(Zayn->window, framebuffer_size_callback);

#if OPENGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
#endif

    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    // Milliseconds
    double startTime = (spec.tv_sec * 1000.0) + (spec.tv_nsec / 1.0e6);
    double gameTime = 0.0;
    double systemTime = startTime;
    double prevSystemTime = systemTime;
    double deltaTime;

    // seconds
    double frameRate = 1.0 / 60.0; // Hertz
    double timeSinceRender = 0.0;


    // GLenum err = glewInit();
    ZaynInit(zaynMemory);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Zayn->window) && zaynPlatform.running)
    {

    //     /* Render here */
    //     /* Swap front and back buffers */

        //  *******************  //
        //  TIME IMPLEMENTATION  //
        //  *******************  //
        clock_gettime(CLOCK_MONOTONIC, &spec);

        prevSystemTime = systemTime;
        systemTime = (spec.tv_sec * 1000.0) + (spec.tv_nsec / 1.0e6);

        deltaTime = (systemTime - prevSystemTime) / 1000.0;

        gameTime = (systemTime - startTime) / 1000.0;

        // seconds
        double dt = deltaTime;

        timeSinceRender += dt;

        Zayn->deltaTime = dt;
        Zayn->time = gameTime;
        Zayn->systemTime = systemTime;


        if (timeSinceRender < frameRate) 
        {
            double timeUntilRender = frameRate - timeSinceRender;
            usleep(timeUntilRender * 1.0e6);
        }

        InputUpdate(Zayn, inputManager);
        ZaynUpdateAndRender(zaynMemory);

        glfwSwapBuffers(Zayn->window);

        zaynPlatform.running = !glfwWindowShouldClose(Zayn->window);

        ClearInputManager(inputManager);

        /* Poll for and process events */

        timeSinceRender = 0.0;

    }

    RenderCleanup();

    glfwDestroyWindow(Zayn->window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    // glViewport(0, 0, width, height);

    // window = glfwCreateWindow(width, height, "Hello World", nullptr, nullptr);

    // glfwGetWindowUserPointer(window)
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    auto zaynMem = reinterpret_cast<ZaynMemory*>(glfwGetWindowUserPointer(window));
    std::cout << "fbscb" << std::endl;
    zaynMem->vkFramebufferResized = true;
    Zayn->windowSize.x = width;
    Zayn->windowSize.y = height;
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
    // app->framebufferResized = true;

}