

void InitCoordinator(Coordinator* coordinator)
{
    InitEntityManager(&coordinator->entityManager);
    InitComponentManager(&coordinator->componentManager);
    InitSystemManager(&coordinator->systemManager);
}

EntityHandle CreateEntity_Coordinator(Coordinator* coordinator)
{
    return CreateEntity(coordinator->entityManager);
}

void DestroyEntity_Coordinator(EntityHandle entityHandle)
{
    DestroyEntity_EntityManager(entityHandle);

    EntityDestroyed_ComponentManager(entityHandle);

    EntityDestroyed_SystemManager(entityHandle);
}

// template<typename T>
void RegisterComponent_Coordinator(ComponentType componentType)
{
    RegisterComponent_ComponentManager(componentType, 24);
}
// void DestroyEntity(Entity entity)
// {
//     // mEntityManager->DestroyEntity(entity);

//     // mComponentManager->EntityDestroyed(entity);

//     // mSystemManager->EntityDestroyed(entity);
// }