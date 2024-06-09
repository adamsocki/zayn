


#define OPENGL 0
#define VULKAN 1


#include <vulkan/vulkan.h>


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#if WINDOWS
#include <cstring>
#endif



#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

#include "data_types.h"

#include "config.h"

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

    glfwInit();
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;    

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    zaynMemory->window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  
    

    glfwMakeContextCurrent(Zayn->window);
    glfwSetFramebufferSizeCallback(Zayn->window, framebuffer_size_callback);

    ZaynInit(zaynMemory);

    while (!glfwWindowShouldClose(Zayn->window) && zaynPlatform.running)
    {
        InputUpdate(Zayn, inputManager);
        ZaynUpdateAndRender(zaynMemory);

        glfwSwapBuffers(Zayn->window);

        zaynPlatform.running = !glfwWindowShouldClose(Zayn->window);

        ClearInputManager(inputManager);
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
    glViewport(0, 0, width, height);
}