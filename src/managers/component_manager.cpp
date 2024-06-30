


void InitComponentManager(ComponentManager* manager)
{
    manager->nextID = 0;
    // ComponentTypeBuffer* fernBuffer = &Data->em.buffers[EntityType_Fern];
	// fernBuffer->capacity = 8000;
	// fernBuffer->sizeInBytes = sizeof(Fern);
	// fernBuffer->count = 0;
	// fernBuffer->entities = (Fern*)malloc(fernBuffer->capacity * fernBuffer->sizeInBytes);
	// memset(fernBuffer->entities, 0, sizeof(Fern) * fernBuffer->capacity);
}


void EntityDestroyed_ComponentManager(EntityHandle& entityHandle)
{

}

// template<typename T>
void RegisterComponent_ComponentManager(ComponentType componentType, int32 capacity)
{
    ComponentTypeBuffer* buffer = &Casette->coordinator.componentManager.componentBuffers[componentType];
    buffer->capacity = capacity;
    buffer->sizeInBytes = sizeof(componentType);
    buffer->count = 0;
    buffer->components = malloc(capacity * sizeof(componentType));
    memset(buffer->components, 0, capacity * sizeof(componentType));


    Casette->coordinator.componentManager.nextID++;
}