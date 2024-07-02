


// void InitComponentManager()
// {
//     Casette->coordinator.componentManager.componentCapacity = 30;
// 	Casette->coordinator.componentManager.components = (ComponentInfo*)malloc(sizeof(ComponentInfo) * Casette->coordinator.componentManager.componentCapacity);
// 	memset(Casette->coordinator.componentManager.components, 0, sizeof(ComponentInfo) * Casette->coordinator.componentManager.componentCapacity);
//     Casette->coordinator.componentManager.nextID = 0;
// }


// void EntityDestroyed_ComponentManager(EntityHandle& entityHandle)
// {

// }

// // template<typename T>
// void RegisterComponent_ComponentManager(ComponentType componentType, int32 capacity)
// {
//     ComponentTypeBuffer* buffer = &Casette->coordinator.componentManager.componentBuffers[componentType];
//     buffer->capacity = capacity;
//     buffer->sizeInBytes = sizeof(componentType);
//     buffer->count = 0;
//     buffer->components = malloc(capacity * sizeof(componentType));
//     memset(buffer->components, 0, capacity * sizeof(componentType));

//     Casette->coordinator.componentManager.nextID++;
// }