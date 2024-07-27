
#include <glm/gtc/matrix_transform.hpp>


void LoadShader()
{

}



void InitCasette(ZaynMemory *zaynMem)
{

    zaynMem->myCasette = malloc(sizeof(MyCasette));
    memset(zaynMem->myCasette, 0, sizeof(MyCasette));
    Casette = (MyCasette*)zaynMem->myCasette;

    AllocateEntityManager(&Casette->em, &zaynMem->permanentMemArena, entityTypeCounts);
    
    // TODO GET ALL CASETTES AVAILABLE
    for (int i = 0; i < 1; i++)
    {
           }


    InGameCameraInit();

    

    Casette_Entity casette = {};
    zaynMem->casette_handle_1 = AddEntity(&Casette->em, Casette_Entity, &casette);
 

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
    // zaynMem->camera.pos = V3(0.0f,0.0f,0.0f);
    // zaynMem->camera.viewMatrix = SetViewDirection(V3(0.0f, 0.0f, 0.0f), V3(0.5f, 0.0f, 1.0f), UP_V);
    // zaynMem->camera.viewMatrix = SetViewTarget(V3(0.0f, 0.0f, testMonkey->posTest), V3(0.0f, 0.0f, 0.53f), UP_V);

    Casette_Entity* cas = GetEntity(&Casette->em, Casette_Entity, zaynMem->casette_handle_1);
    cas->model = &zaynMem->casette_model;
    
    cas->transform3d.translation = V3(3.0f, 0.0f, 0.53f);
    cas->transform3d.scale = V3(1.0f, 1.0f, 1.0f);
    cas->transform3d.rotation = V3(0.3f, 1.0f, 0.4f);
    cas->transform3d.angleRotation = 5.0f;

    
    

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
    
    
    // UpdateInput();

    
    // UpdateCamera();


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
    // zaynMem->camera.type = CameraType_Perspective;
    //   real32 aspect = zaynMem->vkSwapChainExtent.width / zaynMem->vkSwapChainExtent.height;

    // zaynMem->camera.projection = Perspective(DegToRad(60.0f), aspect, 0.1f, 20000.0f);
    // // zaynMem->camera.projection.m11 *= -1;
    // // vec3 cameraPosition = V3(0, 0, -10);
    // quaternion cameraRotation = IdentityQuaternion();
    // Camera* cam = &zaynMem->camera;
    // cam->yaw = -90 - RadToDeg(cam->targetRotY);
    
    // cam->front.x = cosf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    // cam->front.y = sinf(DegToRad(cam->pitch));
    // cam->front.z = sinf(DegToRad(cam->yaw)) * cosf(DegToRad(cam->pitch));
    // cam->front = Normalize(cam->front);


    // mat4 camWorld = TRS(zaynMem->camera.pos, cameraRotation, V3(1));
    // zaynMem->camera.view = OrthogonalInverse(camWorld);
    
    // zaynMem->camera.viewProjection = zaynMem->camera.projection * zaynMem->camera.view;


    // UpdateCameraInGame();

    // zaynMem->camera.viewMatrix = SetViewTarget(V3(0.0f, 0.0f, testMonkey->posTest), V3(0.0f, 0.0f, 0.53f), UP_V);

    //   real32 aspect = zaynMem->vkSwapChainExtent.width / zaynMem->vkSwapChainExtent.height;
    // zaynMem->camera.projectionMatrix = Orthographic(-aspect, aspect, -1, 1, -1, 1);
    // zaynMem->camera.projectionMatrix = (Perspective(glm::radians(60.0f), aspect, 0.1f,13.0f));
    // zaynMem->camera.projectionMatrix = glm::perspective(glm::radians(45.0f), zaynMem->vkSwapChainExtent.width /  zaynMem->vkSwapChainExtent.height, 0.1f, 10.0f);
    Camera* cam = &Zayn->camera;

    if (InputHeld(Keyboard, Input_W))
    {
        cam->pos += cam->front * 0.05f;
    }
    if (InputHeld(Keyboard, Input_S))
    {
        cam->pos -= cam->front * 0.05f;
    }

    UpdateRender_Learn(zaynMem);

    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;




    
}