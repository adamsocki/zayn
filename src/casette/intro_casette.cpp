
#include <glm/gtc/matrix_transform.hpp>


void LoadShader()
{

}



void InitCasette(ZaynMemory *zaynMem)
{

    zaynMem->myCasette = malloc(sizeof(MyCasette));
    memset(zaynMem->myCasette, 0, sizeof(MyCasette));
    Casette = (MyCasette*)zaynMem->myCasette;

    // InitEntityManager();


    // InitCoordinator(&Casette->coordinator);

    // RegisterComponent_Coordinator(ComponentType_Transform);

    
    


    AllocateEntityManager(&Casette->em, &zaynMem->permanentMemArena, entityTypeCounts);

    Monkey firstMonkey = {};
    firstMonkey.testingBool = true;
    Monkey secondMonkey = {};

    Monkey thirdMonkey = {};
    Monkey fourthMonkey = {};
    Monkey fifthMonkey = {};


    zaynMem->monkeyHandle1 = AddEntity(&Casette->em, Monkey, &firstMonkey);
    AddEntity(&Casette->em, Monkey, &secondMonkey);
    AddEntity(&Casette->em, Monkey, &thirdMonkey);
    AddEntity(&Casette->em, Monkey, &fourthMonkey);
    AddEntity(&Casette->em, Monkey, &fifthMonkey);

    

    Monkey* testMonkey = GetEntity(&Casette->em, Monkey, zaynMem->monkeyHandle1);
    testMonkey->model = &zaynMem->model2;
    testMonkey->color = V3(0.1f, 0.2f, 0.6f);
    // testMonkey->transform2d.translation = V2(-0.4f, 0.0f);
    // mat2 sc = Scale2();
    // testMonkey->transform2d.scale = V2(3.4f, 0.12f);
    testMonkey->transform3d.translation = V3(0.0f, 0.0f, 0.53f);
    testMonkey->transform3d.scale = V3(1.0f, 1.0f, 1.0f);
    testMonkey->transform3d.rotation = V3(0.3f, 1.0f, 0.4f);
    testMonkey->transform3d.angleRotation = 5.0f;

    testMonkey->rad = 0.0f;
    

    InitRender_Learn(zaynMem);

    testMonkey->posTest = 0.0f; 
    zaynMem->camera.pos = V3(0.0f,0.0f,0.0f);
    // zaynMem->camera.viewMatrix = SetViewDirection(V3(0.0f, 0.0f, 0.0f), V3(0.5f, 0.0f, 1.0f), UP_V);
    // zaynMem->camera.viewMatrix = SetViewTarget(V3(0.0f, 0.0f, testMonkey->posTest), V3(0.0f, 0.0f, 0.53f), UP_V);



    
    

    if (testMonkey)
    {
        std::cout << "testMonkey" << std::endl;
    }
    else
    {
        std::cout << "No testMonkey" << std::endl;

    }

}


void UpdateAndRenderCasette(ZaynMemory *zaynMem)
{

    // ********
	// LOGIC
	// ********
    // glClear(GL_COLOR_BUFFER_BIT);

    //ClearColor(V4(0.1f, 0.9f, 0.4f, 1.0f));
    
    for (int i = 0; i < Casette->em.entities[EntityType_Monkey].array.count; i++)
    {
        // std::cout << "hi" << std::endl;
        // std::cout << i << std::endl;
    }


    if (InputHeld(Keyboard, Input_W))
    {
        zaynMem->camera.pos.z += 0.004f;
    }

    if (InputHeld(Keyboard, Input_S))
    {
        zaynMem->camera.pos.z -= 0.004f;
    }


    Monkey* testMonkey = GetEntity(&Casette->em, Monkey, zaynMem->monkeyHandle1);
    // if(Inp)

    testMonkey->rad += 0.1f;
    testMonkey->rad += 0.1f;
    testMonkey->transform3d.angleRotation += 0.009f;
    testMonkey->posTest -= 0.01;

    // zaynMem->camera.pos.z -= 1.0f;

    zaynMem->camera.frameCount += 1;

    // Define a base speed and an acceleration factor
    float baseSpeed = 0.0;
    float acceleration = 0.00000f;

    // Calculate the new speed
    float currentSpeed = baseSpeed + (acceleration * zaynMem->camera.frameCount);

    // zaynMem->camera.pos.z -= currentSpeed;

    //    Camera *cam = &Game->camera;
    zaynMem->camera.type = CameraType_Perspective;
    zaynMem->camera.projection = Perspective(DegToRad(80.0f), 16.0f / 9.0f, 0.1f, 1000.0f);

    // vec3 cameraPosition = V3(0, 0, -10);
    quaternion cameraRotation = IdentityQuaternion();

    mat4 camWorld = TRS(zaynMem->camera.pos, cameraRotation, V3(1));
    zaynMem->camera.view = OrthogonalInverse(camWorld);
    
    zaynMem->camera.viewProjection = zaynMem->camera.projection * zaynMem->camera.view;

    // zaynMem->camera.viewMatrix = SetViewTarget(V3(0.0f, 0.0f, testMonkey->posTest), V3(0.0f, 0.0f, 0.53f), UP_V);

    //   real32 aspect = zaynMem->vkSwapChainExtent.width / zaynMem->vkSwapChainExtent.height;
    // zaynMem->camera.projectionMatrix = Orthographic(-aspect, aspect, -1, 1, -1, 1);
    // zaynMem->camera.projectionMatrix = (Perspective(glm::radians(60.0f), aspect, 0.1f,13.0f));
    // zaynMem->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), zaynMem->vkSwapChainExtent.width /  zaynMem->vkSwapChainExtent.height, 0.1f, 10.0f);
   
    if (InputHeld(Keyboard, Input_A))
    {
        std::cout << "A HELD" << std::endl;
    }

   UpdateRender_Learn(zaynMem);



    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;
}