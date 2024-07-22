enum EntityType 
{
    EntityType_Entity, 
    EntityType_Monkey,
    EntityType_Casette_Entity,

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
    mat2 iMatrix2 = Identity2();
    vec2 scale;
    real32 rotation;
};

struct Transform3DComponent
{
    vec3 translation;
    mat4 iMatrix4 = Identity4();
    vec3 scale;
    vec3 rotation;
    real32 angleRotation;
};

struct Entity
{
    EntityHandle handle;
    Model *model;
    vec3 color;
    Transform2DComponent transform2d;
    Transform3DComponent transform3d;
};

struct Player : public Entity {
    // char name[32];  // Player-specific data
    int32 level;
};

struct Casette_Entity : Entity
{

};

struct Monkey : Entity 
{
    bool testingBool;
    real32 rad;
    real32 posTest;

    // float patrolRange;  // Enemy-specific data
};


int32 EntitySizeTable[] = 
{
    sizeof(Entity),
    sizeof(Monkey),
    sizeof(Casette_Entity),
    0,
};

int32 entityTypeCounts[] = 
{
    50, 
    20,
    100
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

