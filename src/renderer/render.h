

#if VULKAN


struct PushConstantData2D
{
    vec2 offset;
    mat2 transform;

    alignas(16) vec3 color;
};

struct PushConstantData3D
{
    // vec2 offset;
    mat4 transform;
    mat4 modelMatrix;

    // alignas(16) vec3 color;
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

// struct Vertex
// {
//     glm::vec2 pos;
//     glm::vec3 color;

//     static VkVertexInputBindingDescription getBindingDescription()
//     {
//         VkVertexInputBindingDescription bindingDescription{};
//         bindingDescription.binding = 0;
//         bindingDescription.stride = sizeof(Vertex);
//         bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

//         return bindingDescription;
//     }

//     static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
//     {
//         std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

//         attributeDescriptions[0].binding = 0;
//         attributeDescriptions[0].location = 0;
//         attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//         attributeDescriptions[0].offset = offsetof(Vertex, pos);

//         attributeDescriptions[1].binding = 0;
//         attributeDescriptions[1].location = 1;
//         attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//         attributeDescriptions[1].offset = offsetof(Vertex, color);

//         return attributeDescriptions;
//     }
// };

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