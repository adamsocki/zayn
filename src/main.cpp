


#define OPENGL 0
#define VULKAN 1


#include <vulkan/vulkan.h>


#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif


#include <iostream>
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>

#include "data_types.h"

#include "config.h"


#if WINDOWS
#include <cstring>
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>


    



struct SystemPlatform
{
    GLFWwindow* window;
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
  
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
   /* Create a windowed mode window and its OpenGL context */
    platform.window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!platform.window)
    {
        glfwTerminate();
        return -1;
    }
  

    /* Make the window's context current */
    glfwMakeContextCurrent(platform.window);
glfwSetFramebufferSizeCallback(platform.window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // GLenum err = glewInit();
    ZaynInit(zaynMemory);

    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(platform.window) && zaynPlatform.running)
    {

    



    //     /* Render here */
    //     /* Swap front and back buffers */


        InputUpdate(&platform, inputManager);
        ZaynUpdateAndRender(zaynMemory);


        glfwSwapBuffers(platform.window);

        zaynPlatform.running = !glfwWindowShouldClose(platform.window);

        ClearInputManager(inputManager);

        /* Poll for and process events */
    }

    RenderCleanup(Zayn);

    glfwDestroyWindow(platform.window);
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}