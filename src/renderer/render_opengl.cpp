






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




inline bool glCheckError_(const char *file, uint32 line) {
    GLenum _glError = glGetError();
    if (_glError != GL_NO_ERROR) {
         std::cout << "OpenGL error (" << file << ":" << line << "): 0x" << std::hex << _glError << " (" << std::dec << _glError << ")\n";

       return true;
    }
    return false;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__);

void LoadShader(const char *vertPath, const char *fragPath, Shader *shader) {
    
//     std::filesystem::path projectRoot = std::filesystem::current_path().parent_path(); 
//     std::filesystem::path vertAbsolutePath = projectRoot / vertPath;
//     std::filesystem::path fragAbsolutePath = projectRoot / fragPath;

//  std::cout << "Project Root: " << projectRoot << std::endl;
//     std::cout << "Absolute Vertex Shader Path: " << vertAbsolutePath << std::endl;
//     std::cout << "Absolute Fragment Shader Path: " << fragAbsolutePath << std::endl;

        FILE *file = fopen(vertPath, "r");
        char string[105];
        if (file == NULL) {
        fprintf(stderr, "can't open %s: %s\n", vertPath, strerror(errno));
    //    exit(1);
    }
    while (fgets(string, 100, file)) {
        printf("%s", string);
    }
        
    if (file != NULL) 
    {
        fseek(file, 0, SEEK_END);
        shader->vertSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        shader->vertSrc = (char *)malloc(shader->vertSize + 1);
        int index = 0;
        int c = fgetc(file);
        while (c != EOF) {
            shader->vertSrc[index++] = c;
            c = fgetc(file);
        }

        shader->vertSrc[index] = 0;

        fclose(file);
    }
    else 
    {
        std::cout << "Error opening file " << vertPath << std::endl; 
    }

    file = fopen(fragPath, "r");
    if (!file)
    {

        perror("fopen");
    }

    if (file != NULL) 
    {
        fseek(file, 0, SEEK_END);
        shader->fragSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        shader->fragSrc = (char *)malloc(shader->fragSize + 1);
        int index = 0;
        int c = fgetc(file);
        while (c != EOF) {
            shader->fragSrc[index++] = c;
            c = fgetc(file);
        }

        shader->fragSrc[index] = 0;

        fclose(file);
    }
    else 
    {
        std::cout << "Error opening file " << fragPath << std::endl;
    }
}

// bool ShaderCompiled(GLuint shader, char **infoLog) {
//     int32 isCompiled = 0;
//     glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
//     if (isCompiled == GL_FALSE) {
//         if (infoLog != NULL) {
//             GLint maxLength = 0;
//             glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

//             maxLength = 256;

//             *infoLog = (GLchar *)malloc(sizeof(GLchar) * maxLength);
//             glGetShaderInfoLog(shader, maxLength, &maxLength, (GLchar *)*infoLog);
//         }
//         std::cout << *infoLog  << std::endl;

        
//         glDeleteShader(shader);
//     }

//     return isCompiled;
// }

// bool ShaderLinked(GLuint shader, char **infoLog) {
//     int32 isLinked = 0;
//     glGetProgramiv(shader, GL_LINK_STATUS, &isLinked);
//     if (isLinked == GL_FALSE) {
//         if (infoLog != NULL) {
//             GLint maxLength = 0;
//             glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

//             maxLength = 256;

//             *infoLog = (GLchar *)malloc(sizeof(GLchar) * maxLength);
//             glGetProgramInfoLog(shader, maxLength, &maxLength, (GLchar *)*infoLog);
//         }

//         std::cout << *infoLog << std::endl;
        
//         glDeleteShader(shader);
//     }

//     return isLinked;
// }

// void CompileShader(Shader *shader, uint32 uniformCount, const char **uniformNames) {
//     char *infoLog = NULL;
    
//     if (shader->vertSrc != NULL) 
//     {
//         shader->vertID = glCreateShader(GL_VERTEX_SHADER);
//         glShaderSource(shader->vertID, 1, (const GLchar**)&shader->vertSrc, (GLint *)&shader->vertSize);
//         glCheckError();
//         glCompileShader(shader->vertID);
//         glCheckError();

//         std::cout << "checking vert shader" << std::endl;

//         ShaderCompiled(shader->vertID, &infoLog);
//     }
//     if (shader->fragSrc != NULL) 
//     {
//         shader->fragID = glCreateShader(GL_FRAGMENT_SHADER);
//         glShaderSource(shader->fragID, 1, (const GLchar**)&shader->fragSrc, (GLint *)&shader->fragSize);
//         glCheckError();
//         glCompileShader(shader->fragID);
//         glCheckError();

//         std::cout << "checking frag shader" << std::endl;
//         ShaderCompiled(shader->fragID, &infoLog);   /// may actually be vert id
//     }


//     shader->programID = glCreateProgram();

//     if (!glIsProgram(shader->programID)) {
//         std::cout << "NOT A SHADER!" << std::endl;
//     }
    
//     if (shader->vertID != 0) {
//         glAttachShader(shader->programID, shader->vertID);
//         glCheckError();
//     }
//     if (shader->fragID != 0) {
//         glAttachShader(shader->programID, shader->fragID);
//         glCheckError();
//     }

//     glLinkProgram(shader->programID);
//     glCheckError();

//     ShaderLinked(shader->programID, &infoLog);
    
    
//     shader->uniformCount = uniformCount;

//     shader->uniforms = (ShaderUniform *)malloc(sizeof(ShaderUniform) * uniformCount);
    
//     for (int i = 0; i < uniformCount; i++) 
//     {
//         ShaderUniform *uniform = &shader->uniforms[i];
//         const char *uniformName = uniformNames[i];

//         uint32 nameLen = strlen(uniformName);
//         uniform->name = (char *)malloc(nameLen + 1);
//         memcpy(uniform->name, uniformName, nameLen);
//         uniform->name[nameLen] = 0;
        
//         uniform->id = glGetUniformLocation(shader->programID, uniform->name);
//         glCheckError();
        
//         if (uniform->id >= 0) 
//         {
//             std::cout << "Setting uniform " << uniform->name << std::endl;

//         }
//         else 
//         {
//             std::cout << "failed to set " << uniform->name << std::endl;
//         }
//         ShaderCompiled(shader->vertID, &infoLog);
//     }
// }




void SetShader(Shader *shader) {
    glUseProgram(shader->programID);
    //glCheckError();
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


bool ShaderCompiled(GLuint shader, char **infoLog)
{
    int32 isCompiled = 0; 

    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
    if (isCompiled == GL_FALSE)
    {
        if (infoLog != NULL)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
            maxLength = 256; 

             *infoLog = (GLchar *)malloc(sizeof(GLchar) * maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, (GLchar *)*infoLog);

        }

        std::cout << *infoLog << std::endl;

        glDeleteShader(shader);

    }
    return isCompiled;

}

bool ShaderLinked(GLuint shader, char **infoLog)
{
    int32 isLinked = 0;
    glGetProgramiv(shader, GL_LINK_STATUS, &isLinked);
    
    if (isLinked == GL_FALSE) 
    {
        if (infoLog != NULL) 
        {
            GLint maxLength = 0;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            maxLength = 256;

            *infoLog = (GLchar *)malloc(sizeof(GLchar) * maxLength);
            glGetProgramInfoLog(shader, maxLength, &maxLength, (GLchar *)*infoLog);
        }

        std::cout << *infoLog << std::endl;

        glDeleteShader(shader);
    }

    return isLinked;
}

void CompileShader(Shader *shader)
{
    char *infoLog = NULL;

    if (shader->vertSrc != NULL)
    {

        shader->vertID = glCreateShader(GL_VERTEX_SHADER);

        glShaderSource(shader->vertID, 1, (const GLchar**)&shader->vertSrc, (GLint *)&shader->vertSize);
        glCheckError();
        glCompileShader(shader->vertID);
        glCheckError();

        ShaderCompiled(shader->vertID, &infoLog);
    }

    if (shader->fragSrc != NULL)
    {

        shader->fragID = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(shader->fragID, 1, (const GLchar**)&shader->fragSrc, (GLint *)&shader->fragSize);
        glCheckError();
        glCompileShader(shader->fragID);
        glCheckError();

        ShaderCompiled(shader->fragID, &infoLog);
    }

    shader->programID = glCreateProgram();

    if (!glIsProgram(shader->programID)) 
    {
        std::cout << "NOT A SHADER!" << std::endl;
    }
     if (shader->vertID != 0) 
     {
        glAttachShader(shader->programID, shader->vertID);
        glCheckError();
    }
    if (shader->fragID != 0) 
    {
        glAttachShader(shader->programID, shader->fragID);
        glCheckError();
    }

    glLinkProgram(shader->programID);
    glCheckError();


   ShaderLinked(shader->programID, &infoLog);
}


void InitRender_Learn(ZaynMemory *zaynMem)
{

    
    std::cout << "hey" << std::endl;
    Shader *firstShader = &zaynMem->shaderCollection.shader_test_01;
     // build and compile our shader program
    // ------------------------------------
    // vertex shader
// src\renderer\shaders\mesh.vert   
    // load START
    // LoadShader("../shaders/mesh.vert", "../shaders/mesh.frag", firstShader);
    LoadShader("/Users/socki/dev/zayn/src/renderer/shaders/mesh.vert", "/Users/socki/dev/zayn/src/renderer/shaders/mesh.frag", firstShader);
    // LoadShader("C:/dev/zayn/src/renderer/shaders/mesh.vert", "C:/dev/zayn/src/renderer/shaders/mesh.frag", firstShader);

    // END LOAD
    
    // src/renderer/shaders/mesh.vert
    // CompileShader(firstShader, ) 

    // zaynMem->shaderCollection.shader_test_01.vertID = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(zaynMem->shaderCollection.shader_test_01.vertID, 1, &vertexShaderSource, NULL);
    // glCompileShader(zaynMem->shaderCollection.shader_test_01.vertID);

    // COMPLE START

    CompileShader(firstShader);
    

    // check for shader compile errors
    // int success;
    // char infoLog[512];
    // glGetShaderiv(zaynMem->shaderCollection.shader_test_01.vertID, GL_COMPILE_STATUS, &success);

    // if (!success)
    // {
    //     glGetShaderInfoLog(zaynMem->shaderCollection.shader_test_01.vertID, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    // fragment shader
    // zaynMem->shaderCollection.shader_test_01.fragID = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(zaynMem->shaderCollection.shader_test_01.fragID, 1, &fragmentShaderSource, NULL);
    // glCompileShader(zaynMem->shaderCollection.shader_test_01.fragID);
    // // check for shader compile errors
    // glGetShaderiv(zaynMem->shaderCollection.shader_test_01.fragID, GL_COMPILE_STATUS, &success);
    // if (!success)
    // {
    //     glGetShaderInfoLog(zaynMem->shaderCollection.shader_test_01.fragID, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    // }
    // link shaders
    //  zaynMem->shaderCollection.shader_test_01.programID = glCreateProgram();
    // glAttachShader(zaynMem->shaderCollection.shader_test_01.programID, zaynMem->shaderCollection.shader_test_01.vertID);
    // glAttachShader(zaynMem->shaderCollection.shader_test_01.programID, zaynMem->shaderCollection.shader_test_01.fragID);
    // glLinkProgram(zaynMem->shaderCollection.shader_test_01.programID);
    // // check for linking errors
    // glGetProgramiv(zaynMem->shaderCollection.shader_test_01.programID, GL_LINK_STATUS, &success);
    // if (!success) {
    //     glGetProgramInfoLog(zaynMem->shaderCollection.shader_test_01.programID, 512, NULL, infoLog);
    //     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    // }
    // glDeleteShader(zaynMem->shaderCollection.shader_test_01.vertID);
    // glDeleteShader(zaynMem->shaderCollection.shader_test_01.fragID);

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
        1, 2, 3, // second Triangle
        4, 2, 1  // 3o Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &firstShader->VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(firstShader->VAO);

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

    glUseProgram(zaynMem->shaderCollection.shader_test_01.programID);
    glBindVertexArray(zaynMem->shaderCollection.shader_test_01.VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES,9, GL_UNSIGNED_INT, 0);
    
}


void RenderCleanup(ZaynMemory* zaynMem)
{

}