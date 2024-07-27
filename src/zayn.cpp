

#include <mach/mach_time.h>
#include "hash.cpp"
#include "time.cpp"

#include "zayn.h"

#include "buffer.cpp"

#include "camera.cpp"

#include "entity.cpp"



#include "model.cpp"


#include "renderer/mesh.cpp"
#include "renderer/render.cpp"

#include "memory.cpp"

#include "input.cpp"



#include "camera_manager.cpp"

#include "casette/casette.cpp"


void ZaynInit(ZaynMemory *zaynMem)
{
    Zayn = zaynMem;
    Input = &zaynMem->inputManager;

    uint64_t startTime = mach_absolute_time();

    InitCasette(zaynMem);
}



void ZaynUpdateAndRender(ZaynMemory *zaynMem)
{



   UpdateAndRenderCasette(zaynMem);

}