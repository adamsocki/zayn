


#include "zayn.h"

#include "model.cpp"

#include "renderer/mesh.cpp"

#include "renderer/render.cpp"



#include "memory.cpp"

#include "input.cpp"


#include "cartridge/cartridge_code.cpp"


void ZaynInit(ZaynMemory *zaynMem)
{
    Zayn = zaynMem;
    Input = &zaynMem->inputManager;

  

    // AllocateMemoryArena(&Zayn->permanentMemArena, Megabytes(16));
    // AllocateMemoryArena(&Zayn->frameMemArena, Megabytes(16));
    
    
    // AllocateTriangleMesh(&zaynMem->meshCollection.triangleMesh);
    // //InitMesh(&zaynMem->meshCollection.triangleMesh);
    // AllocateQuadMesh(&zaynMem->meshCollection.quadMesh);
    //InitMesh(&zaynMem->meshCollection.quadMesh);
    

    // AllocateTriangle(&zaynMem->triangleMesh, zaynMem);

    
    InitCartridge(zaynMem);
    // AllocateQuad(&zaynMem->quadMesh, zaynMem);


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

    // glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    // glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    // glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    // glm::vec4 color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

    // // DrawMesh(zaynMem, &triangleMesh, pos, rotation, scale, color, false);
    // DrawMesh(zaynMem, &zaynMem->quadMesh, pos, rotation, scale, color, false);

    UpdateAndRenderCartridge(zaynMem);

}