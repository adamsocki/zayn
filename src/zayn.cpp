


#include "zayn.h"

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
    
    InitCartridge(zaynMem);

}



void ZaynUpdateAndRender(ZaynMemory *zaynMem)
{



    UpdateAndRenderCartridge(zaynMem);
}