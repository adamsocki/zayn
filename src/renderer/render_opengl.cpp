






void InitMesh(Mesh *mesh) {
    GLuint vertexBuffer;

    glGenBuffers(1, &vertexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ARRAY_BUFFER, mesh->size, mesh->data, GL_STATIC_DRAW);

    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    // The following commands will talk about our 'vertexbuffer' buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    // Give our vertices to OpenGL.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indexCount * sizeof(uint32), mesh->indices, GL_STATIC_DRAW);

    mesh->vertBufferID = vertexBuffer;
    mesh->indexBufferID = indexBuffer;
}







//LEARN CODE
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";



void ClearColor(vec4 color) 
{
    glClearColor(color.r, color.g, color.b, color.a);
}



void InitRender_Learn(ZaynMemory *zaynMem)
{


     // build and compile our shader program
    // ------------------------------------
    // vertex shader
     zaynMem->shaderCollection.shader_test_01.vertID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(zaynMem->shaderCollection.shader_test_01.vertID, 1, &vertexShaderSource, NULL);
    glCompileShader(zaynMem->shaderCollection.shader_test_01.vertID);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(zaynMem->shaderCollection.shader_test_01.vertID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zaynMem->shaderCollection.shader_test_01.vertID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
     zaynMem->shaderCollection.shader_test_01.fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(zaynMem->shaderCollection.shader_test_01.fragID, 1, &fragmentShaderSource, NULL);
    glCompileShader(zaynMem->shaderCollection.shader_test_01.fragID);
    // check for shader compile errors
    glGetShaderiv(zaynMem->shaderCollection.shader_test_01.fragID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(zaynMem->shaderCollection.shader_test_01.fragID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
     zaynMem->shaderCollection.shader_test_01.programID = glCreateProgram();
    glAttachShader(zaynMem->shaderCollection.shader_test_01.programID, zaynMem->shaderCollection.shader_test_01.vertID);
    glAttachShader(zaynMem->shaderCollection.shader_test_01.programID, zaynMem->shaderCollection.shader_test_01.fragID);
    glLinkProgram(zaynMem->shaderCollection.shader_test_01.programID);
    // check for linking errors
    glGetProgramiv(zaynMem->shaderCollection.shader_test_01.programID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(zaynMem->shaderCollection.shader_test_01.programID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(zaynMem->shaderCollection.shader_test_01.vertID);
    glDeleteShader(zaynMem->shaderCollection.shader_test_01.fragID);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f, // bottom left
        -0.9f,  0.9f, 0.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3, 
        4, 2, 1  // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &zaynMem->shaderCollection.shader_test_01.VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(zaynMem->shaderCollection.shader_test_01.VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0); 

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0); 
}


void UpdateRender_Learn(ZaynMemory *zaynMem)
{
    glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(zaynMem->shaderCollection.shader_test_01.programID);
        glBindVertexArray(zaynMem->shaderCollection.shader_test_01.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT, 0);
    
}