


#include "zayn.h"

#include "renderer/mesh.cpp"
#include "renderer/render.cpp"

#include "memory.cpp"

#include "input.cpp"


#include "cartridge/cartridge_code.cpp"


void ZaynInit(ZaynMemory *zaynMem)
{
    Zayn = zaynMem;
    Input = &zaynMem->inputManager;

  

    AllocateMemoryArena(&Zayn->permanentMemArena, Megabytes(16));
    AllocateMemoryArena(&Zayn->frameMemArena, Megabytes(16));
    
    
    AllocateTriangleMesh(&zaynMem->meshCollection.triangleMesh);
    InitMesh(&zaynMem->meshCollection.triangleMesh);
    AllocateQuadMesh(&zaynMem->meshCollection.quadMesh);
    InitMesh(&zaynMem->meshCollection.quadMesh);
    

    
    InitCartridge(zaynMem);

    {
        // LoadShader("renderer/shaders/mesh.vert", "renderer/shaders/mesh.frag", &zaynMem->shaderCollection.shader_test_01);
        // const char *uniforms[] = {
        //     "model",
        //     "viewProjection",
        //     "color",
        // };
        // CompileShader(&zaynMem->shaderCollection.shader_test_01, 3, uniforms);
    }



}



void ZaynUpdateAndRender(ZaynMemory *zaynMem)
{



    UpdateAndRenderCartridge(zaynMem);

}