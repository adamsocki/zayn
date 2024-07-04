enum EntityType 
{
    EntityType_Entity, 
    EntityType_Monkey,

    EntityType_Count 
};




enum EntityFlag
{
    EntityFlag_None = 0,
    EntityFlag_Deleted = 1 << 2,
};

struct EntityTypeStorage 
{
    EntityType type;
    int32 elementSize;

    DynamicArray_Untyped array;
};

struct EntityInfo
{
    EntityType type;
    int32 generation;

    int32 index;
    EntityFlag flags;
};

struct EntityHandle
{
    EntityType type;
    uint32 id;
    int32 generation;
};

struct Transform2DComponent
{
    vec2 translation;
    // mat2 
};

struct Entity
{
    EntityHandle handle;
    Model *model;
    vec3 color;
};

struct Player : public Entity {
    // char name[32];  // Player-specific data
    int32 level;
};

struct Monkey : Entity 
{
    bool testingBool;

    // float patrolRange;  // Enemy-specific data
};


int32 EntitySizeTable[] = 
{
    sizeof(Entity),
    sizeof(Monkey),
    0,
};

int32 entityTypeCounts[] = 
{
    50, 
    20
}; 



struct EntityManager
{
    DynamicArray<EntityInfo> entityInfo;
    DynamicArray<uint32> freelist;
    DynamicArray<EntityHandle> entitiesToDelete;

    EntityTypeStorage entities[EntityType_Count];

};


struct System 
{
    DynamicArray<EntityHandle> entities;
};

