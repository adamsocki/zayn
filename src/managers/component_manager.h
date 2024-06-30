
struct ComponentTypeBuffer 
{
	int32 count;
	int32 capacity;
	int32 sizeInBytes;

	void* components;
};

// int32MAX_COMPONENTS = 32;

struct ComponentInfo 
{
	int32 generation;
	int32 indexInBuffer;

	ComponentType type;
};




struct ComponentManager
{
    ComponentTypeBuffer componentBuffers[ComponentType_Count];

    int32 nextID;
};