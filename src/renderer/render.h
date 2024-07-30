

#if VULKAN


struct PushConstantData2D
{
    vec2 offset;
    mat2 transform;

    alignas(16) vec3 color;
};

struct PushConstantData3D
{
    mat4 modelMatrix;
};


struct PipelineConfigInfo
{
    // VkViewport viewport;
    // VkRect2D scissor;
    
    // VkPipelineViewportStateCreateInfo viewportStateInfo;
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
    VkPipelineRasterizationStateCreateInfo rasterizationInfo;
    VkPipelineMultisampleStateCreateInfo multisampleInfo;
    VkPipelineColorBlendAttachmentState colorBlendAttachment;
    VkPipelineColorBlendStateCreateInfo colorBlendInfo;
    VkPipelineDepthStencilStateCreateInfo depthStencilInfo;

    // replacing viewport and scissor
    std::vector<VkDynamicState> dynamicStateEnables;
    VkPipelineDynamicStateCreateInfo dynamicStateInfo;

    VkPipelineLayout pipelineLayout = nullptr;
    VkRenderPass renderPass = nullptr;
    uint32_t subpass = 0;
    ;
};



#endif

struct VKRenderData
{
    /* data */
};

struct ShaderUniform
{
    char *name;
    int32 id;
};

struct Shader
{
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