


void LoadShader()
{

}


void InitCasette(ZaynMemory *zaynMem)
{

    zaynMem->myCasette = malloc(sizeof(MyCasette));
    memset(zaynMem->myCasette, 0, sizeof(MyCasette));
    Casette = (MyCasette*)zaynMem->myCasette;

    InitCoordinator(&Casette->coordinator);

    RegisterComponent_Coordinator(ComponentType_Transform);



    
    
    InitRender_Learn(zaynMem);




}




void UpdateAndRenderCasette(ZaynMemory *zaynMem)
{

    // ********
	// LOGIC
	// ********
    // glClear(GL_COLOR_BUFFER_BIT);

    //ClearColor(V4(0.1f, 0.9f, 0.4f, 1.0f));
    


   UpdateRender_Learn(zaynMem);
    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;
}