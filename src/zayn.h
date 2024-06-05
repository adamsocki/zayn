

#include "commonlib.h"

#include "math/math.h"

#include "renderer/mesh.h"
#include "renderer/render.h"

#include "memory.h"

#include "dynamic_array.h"

#include "camera.h"



#include "input.h"



struct MeshCollection
{
    Mesh triangleMesh;
    Mesh quadMesh;
};

struct ShaderCollection
{
    Shader shader_test_01;

};


struct ZaynMemory
{
    InputManager inputManager;
    InputDevice* keyboard;
    InputDevice* mouse;

    MemoryArena permanentMemArena;
    MemoryArena frameMemArena;

    ShaderCollection shaderCollection;
    MeshCollection meshCollection;


};


struct ZaynPlatform
{
    ZaynMemory zaynMemory;
    bool running;
};



ZaynMemory *Zayn = NULL;

InputManager *Input = NULL;
