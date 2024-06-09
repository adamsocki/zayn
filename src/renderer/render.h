
struct VKRenderData
{
    /* data */
};




struct ShaderUniform {
    char *name;
    int32 id;
};

struct Shader {
    uint32 vertSize;
    char *vertSrc;
    
    uint32 fragSize;
    char *fragSrc;
    
    int32 vertID;
    int32 fragID;
    int32 programID;

    int32 uniformCount;
    ShaderUniform *uniforms;

    unsigned int VBO, VAO, EBO;

// put on mesh instead of in shader
    int32 vertBufferID;
    int32 indexBufferID;
};