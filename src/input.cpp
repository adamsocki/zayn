


void AllocateInputManager(InputManager *inputManager, MemoryArena *arena, int32 deviceCapacity)
{
    inputManager->events = MakeDynamicArray<InputEvent>(arena, deviceCapacity);

    inputManager->deviceCount = deviceCapacity;
    inputManager->devices     = (InputDevice *)malloc(sizeof(InputDevice) * deviceCapacity);
    memset(inputManager->devices, 0, inputManager->deviceCount * sizeof(InputDevice));


}


void AllocateInputDevice(InputDevice *device, InputDeviceType type, int32 discreteCount, int32 analogueCount)
{
    device->type = type;
    device->discreteCount = discreteCount;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        InputEvent event = {};
        event.device = Zayn->keyboard;
        event.index = key;
        event.discreteValue = true;

        PushBack(&Input->events, event);

        std::cout << "Key pressed: " << key << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        InputEvent event = {};
        event.device = Zayn->keyboard;
        event.index = key;
        event.discreteValue = false;
        std::cout << "Key released: " << key << std::endl;
    }
}


void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{

}

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
  // std::cout << "Key pressed: " << xpos << std::endl;

}


void InputUpdate(ZaynMemory* zaynMem, InputManager *inputManager)
{
    glfwPollEvents();

    glfwSetKeyCallback(zaynMem->window, keyCallback);
    glfwSetMouseButtonCallback(zaynMem->window, mouseButtonCallback);
    glfwSetCursorPosCallback(zaynMem->window, cursorPosCallback);


    for (int d = 0; d < inputManager->deviceCount; d++)
    {
        InputDevice *device = &inputManager->devices[d];

        for (int i = 0; i < device->discreteCount; i++)
        {

        }
    }



}





void ClearInputManager(InputManager *inputManager)
{
    DynamicArrayClear(&inputManager->events);

    inputManager->charCount = 0;
}
