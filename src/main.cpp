

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#endif

#include <iostream>
#include "data_types.h"

#include "config.h"


#include <GLFW/glfw3.h>


    



struct SystemPlatform
{
    GLFWwindow* window;
    bool running;
};







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
    ZaynInit(zaynMemory);


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    platform.window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!platform.window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(platform.window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(platform.window) && zaynPlatform.running)
    {

        glfwPollEvents();
        

        InputUpdate(&platform, inputManager);




        ZaynUpdate(zaynMemory);



        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(platform.window);


        zaynPlatform.running = !glfwWindowShouldClose(platform.window);

        ClearInputManager(inputManager);

        /* Poll for and process events */
    }

    glfwTerminate();
    return 0;
}