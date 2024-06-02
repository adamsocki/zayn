
enum InputDeviceType
{
    InputDeviceType_Keyboard,
    InputDeviceType_Mouse
};


enum InputKeyboardDiscrete : uint32
{
    Input_A,
    Input_KeyboardDiscreteCount
};

struct InputDevice
{
    InputDeviceType type;

    int32 discreteCount;

    // arrays of discrete parts of device (ex. each key)
    int32* framesHeld;
    bool* released;
    bool* pressed;
    real32* timePressed;
};


struct InputEvent
{
    InputDevice *device;
    int32 index;
    bool discreteValue;
    real32 value;

};

struct InputManager
{
    int32 deviceCount;
    InputDevice *devices;

    DynamicArray<InputEvent> events;

    uint32 charCount;

};
