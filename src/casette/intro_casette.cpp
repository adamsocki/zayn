


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


    EntityHandle monkeyHandle = AddEntity(&Casette->em, Monkey, &firstMonkey);
    AddEntity(&Casette->em, Monkey, &secondMonkey);
    AddEntity(&Casette->em, Monkey, &thirdMonkey);
    AddEntity(&Casette->em, Monkey, &fourthMonkey);
    AddEntity(&Casette->em, Monkey, &fifthMonkey);

    
    InitRender_Learn(zaynMem);

    
    Monkey* testMonkey = GetEntity(&Casette->em, Monkey, monkeyHandle);

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
        std::cout << "hi" << std::endl;
        std::cout << i << std::endl;
    }



   UpdateRender_Learn(zaynMem);
    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;
}