


#define OPENGL 0
#define VULKAN 1


#include <vulkan/vulkan.h>
#include <vulkan/vulkan_beta.h>


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
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

    // Keyboard =


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


    Zayn->windowSize.x = 2640;
    Zayn->windowSize.y = 1640;

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

    // GLenum err = glewInit();
    ZaynInit(zaynMemory);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(Zayn->window) && zaynPlatform.running)
    {

    



    //     /* Render here */
    //     /* Swap front and back buffers */


        InputUpdate(Zayn, inputManager);
        ZaynUpdateAndRender(zaynMemory);


        glfwSwapBuffers(Zayn->window);

        zaynPlatform.running = !glfwWindowShouldClose(Zayn->window);

        ClearInputManager(inputManager);

        /* Poll for and process events */
    }

    RenderCleanup(Zayn);

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
    // auto app = reinterpret_cast<HelloTriangleApplication*>(glfwGetWindowUserPointer(window));
        // app->framebufferResized = true;
}