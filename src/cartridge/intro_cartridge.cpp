




void LoadShader()
{

}


void InitCartridge(ZaynMemory *zaynMem)
{
    InitRender_Learn(zaynMem);

    // 1 INit Shader
    // LoadShader();
}




void UpdateAndRenderCartridge(ZaynMemory *zaynMem)
{

    // ********
	// LOGIC
	// ********
    glClear(GL_COLOR_BUFFER_BIT);

    ClearColor(V4(0.1f, 0.9f, 0.4f, 1.0f));
    


    UpdateRender_Learn(zaynMem);
    // ********
	// RENDER
	// ********
    // std::cout << "hi" << std::endl;
}