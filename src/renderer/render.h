

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




#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

#if WINDOWS
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
#elif MAC
const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME, "VK_KHR_portability_subset"};
#endif

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};



static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};
