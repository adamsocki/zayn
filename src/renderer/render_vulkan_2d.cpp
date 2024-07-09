#include <vector>
#include <map>
#include <set>
#include <fstream>

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

const int MAX_FRAMES_IN_FLIGHT = 2;

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo, const VkAllocationCallbacks *pAllocator, VkDebugUtilsMessengerEXT *pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, debugMessenger, pAllocator);
    }
}

const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

bool checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    // std::vector<VkLayerProperties> availableLayers(layerCount);
    VkLayerProperties *availableLayers = (VkLayerProperties *)malloc(layerCount * sizeof(VkLayerProperties));

    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (const char *layerName : validationLayers)
    {
        bool layerFound = false;

        // for (const auto &layerProperties : availableLayers)
        for (int j = 0; j < layerCount; j++)
        {
            if (strcmp(layerName, availableLayers[j].layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if (!layerFound)
        {
            free(availableLayers);
            return false;
        }
    }

    free(availableLayers);

    return true;
}

std::vector<const char *> getRequiredExtensions()
{
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers)
    {
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        extensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    }

#if MAC
    uint32_t instanceExtensionCount;
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);
    std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, availableInstanceExtensions.data());
    for (const auto &extension : availableInstanceExtensions)
    {
        if (strcmp(extension.extensionName, VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == 0)
        {
            extensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
            break;
        }
    }
#endif

    return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
    void *pUserData)
{
    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
    return VK_FALSE;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

void SetupDebugMessenger(ZaynMemory *zaynMem)
{
    if (!enableValidationLayers)
    {
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);

    VkAllocationCallbacks pAllocator = {};

    if (CreateDebugUtilsMessengerEXT(zaynMem->vkInstance, &createInfo, nullptr, &zaynMem->vkDebugMessenger) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

int rateDeviceSuitability(VkPhysicalDevice device)
{
    // ...

    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    int score = 0;
    std::cout << "Device prop type : " << deviceProperties.deviceType << std::endl;
    // Discrete GPUs have a significant performance advantage
    if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
    {
        score += 1000;
        std::cout << "score should inc 1000 : " << std::endl;
    }

    // Maximum possible size of textures affects graphics quality
    score += deviceProperties.limits.maxImageDimension2D;
    std::cout << "deviceProperties.limits.maxImageDimension2D : " << deviceProperties.limits.maxImageDimension2D << std::endl;

// Application can't function without geometry shaders
#ifdef __APPLE__ // Or another platform-specific check
    // Shader code without geometry shaders

#else
    if (!deviceFeatures.geometryShader)
    {
        std::cout << "!deviceFeatures.geometryShader : " << std::endl;

        return 0;
    }
// Shader code with geometry shaders
#endif

    return score;
}

struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete()
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto &queueFamily : queueFamilies)
    {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, zaynMem->vkSurface, &presentSupport);

        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }

        i++;
    }

    return indices;
}

bool checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    for (const auto &extension : availableExtensions)
    {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, zaynMem->vkSurface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, zaynMem->vkSurface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, zaynMem->vkSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, zaynMem->vkSurface, &presentModeCount, nullptr);

    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, zaynMem->vkSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
}

bool isDeviceSuitable(VkPhysicalDevice device, ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices = findQueueFamilies(device, zaynMem);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    bool swapChainAdequate = false;
    if (extensionsSupported)
    {
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(device, zaynMem);
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate;
}

void printDeviceExtensions(VkPhysicalDevice device)
{
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::cout << "Available extensions:\n";
    for (const auto &extension : availableExtensions)
    {
        std::cout << "\t" << extension.extensionName << "\n";
    }
}

void PickPhysicalDevice(ZaynMemory *zaynMem)
{

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(zaynMem->vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0)
    {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(zaynMem->vkInstance, &deviceCount, devices.data());

    for (const auto &device : devices)
    {
        // printDeviceExtensions(device);
        if (isDeviceSuitable(device, zaynMem))
        {
            zaynMem->vkPhysicalDevice = device;
            break;
        }
    }

    if (zaynMem->vkPhysicalDevice == VK_NULL_HANDLE)
    {
        throw std::runtime_error("failed to find a suitable GPU!");
    }
}

void CreateVKInstance(ZaynMemory *zaynMem)
{
    if (enableValidationLayers && !checkValidationLayerSupport())
    {
        std::cerr << "Validation layers requested, but not available!" << std::endl;
        std::exit(EXIT_FAILURE);
    }

    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Zayn";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Zayn Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    createInfo.pApplicationInfo = &appInfo;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();

        populateDebugMessengerCreateInfo(debugCreateInfo);
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else
    {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    if (vkCreateInstance(&createInfo, nullptr, &zaynMem->vkInstance) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create instance!");
    }
}

void CreateLogicalDevice(ZaynMemory *zaynMem)
{
    QueueFamilyIndices indices = findQueueFamilies(zaynMem->vkPhysicalDevice, zaynMem);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies)
    {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    if (enableValidationLayers)
    {
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else
    {
        createInfo.enabledLayerCount = 0;
    }

    if (vkCreateDevice(zaynMem->vkPhysicalDevice, &createInfo, nullptr, &zaynMem->vkDevice) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create logical device!");
    }

    vkGetDeviceQueue(zaynMem->vkDevice, indices.graphicsFamily.value(), 0, &zaynMem->vkGraphicsQueue);
    vkGetDeviceQueue(zaynMem->vkDevice, indices.presentFamily.value(), 0, &zaynMem->vkPresentQueue);
}

void CreateSurface(ZaynMemory *zaynMem)
{
    if (glfwCreateWindowSurface(zaynMem->vkInstance, zaynMem->window, nullptr, &zaynMem->vkSurface) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create window surface!");
    }
}

static std::vector<char> readFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open())
    {
        throw std::runtime_error("failed to open file!");
    }

    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule createShaderModule(const std::vector<char> &code, ZaynMemory *zaynMem)
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data()); // @TODO: remove this when c style refactoring

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(zaynMem->vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

PipelineConfigInfo MakeMyDefaultPipelineConfig(ZaynMemory *zaynMem, uint32_t width, uint32_t height)
{
    PipelineConfigInfo pipelineConfigInfo = {};

    // Input Assembly -> Determines what the vertex inputs mean and how to create geometry. (3 verticies makezz a triangle)
    pipelineConfigInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineConfigInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipelineConfigInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

    // Tells our renderer how to position our image relative to our output window
    // pipelineConfigInfo.viewport.x = 0.0f;
    // pipelineConfigInfo.viewport.y = 0.0f;
    // pipelineConfigInfo.viewport.width = static_cast<float>(width);
    
    // pipelineConfigInfo.viewport.width = (float)zaynMem->windowSize.x;
    // pipelineConfigInfo.viewport.height = static_cast<float>(height);
    
    // pipelineConfigInfo.viewport.height = (float)zaynMem->windowSize.y;
    // pipelineConfigInfo.viewport.minDepth = 0.0f;
    // pipelineConfigInfo.viewport.maxDepth = 1.0f;

    // cutoff on the viewport for rendering
    // pipelineConfigInfo.scissor.offset = {0, 0};
    // pipelineConfigInfo.scissor.extent = {width, height};

    // rasterization breaks up geometry into fragemnts (to later become pixels)
    pipelineConfigInfo.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipelineConfigInfo.rasterizationInfo.depthClampEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineConfigInfo.rasterizationInfo.lineWidth = 1.0f;
    pipelineConfigInfo.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;        // will come back to this
    pipelineConfigInfo.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE; // deterimines which side of the face of triangle is forward on draw
    pipelineConfigInfo.rasterizationInfo.depthBiasEnable = VK_FALSE;
    pipelineConfigInfo.rasterizationInfo.depthBiasConstantFactor = 0.0f; // Optional
    pipelineConfigInfo.rasterizationInfo.depthBiasClamp = 0.0f;          // Optional
    pipelineConfigInfo.rasterizationInfo.depthBiasSlopeFactor = 0.0f;    // Optional

    // multi sample anti aliasing
    pipelineConfigInfo.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineConfigInfo.multisampleInfo.sampleShadingEnable = VK_FALSE;
    pipelineConfigInfo.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineConfigInfo.multisampleInfo.minSampleShading = 1.0f;          // Optional
    pipelineConfigInfo.multisampleInfo.pSampleMask = nullptr;            // Optional
    pipelineConfigInfo.multisampleInfo.alphaToCoverageEnable = VK_FALSE; // Optional
    pipelineConfigInfo.multisampleInfo.alphaToOneEnable = VK_FALSE;      // Optional

    // pipelineConfigInfo.colorBlendAttachment.colorWriteMask =
    //     VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
    //     VK_COLOR_COMPONENT_A_BIT;
    // pipelineConfigInfo.colorBlendAttachment.blendEnable = VK_FALSE;
    // pipelineConfigInfo.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
    // pipelineConfigInfo.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    // pipelineConfigInfo.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
    // pipelineConfigInfo.colorBlendAttachment.srcAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE;  // Optional
    // pipelineConfigInfo.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    // pipelineConfigInfo.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional

    // pipelineConfigInfo.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // pipelineConfigInfo.colorBlendInfo.logicOpEnable = VK_FALSE;
    // pipelineConfigInfo.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    // pipelineConfigInfo.colorBlendInfo.attachmentCount = 1;
    // pipelineConfigInfo.colorBlendInfo.pAttachments = &pipelineConfigInfo.colorBlendAttachment;
    // pipelineConfigInfo.colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    // pipelineConfigInfo.colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    // pipelineConfigInfo.colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    // pipelineConfigInfo.colorBlendInfo.blendConstants[3] = 0.0f; // Optional

    pipelineConfigInfo.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineConfigInfo.depthStencilInfo.depthTestEnable = VK_TRUE;
    pipelineConfigInfo.depthStencilInfo.depthWriteEnable = VK_TRUE;
    pipelineConfigInfo.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
    pipelineConfigInfo.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
    pipelineConfigInfo.depthStencilInfo.minDepthBounds = 0.0f; // Optional
    pipelineConfigInfo.depthStencilInfo.maxDepthBounds = 1.0f; // Optional
    pipelineConfigInfo.depthStencilInfo.stencilTestEnable = VK_FALSE;
    pipelineConfigInfo.depthStencilInfo.front = {}; // Optional
    pipelineConfigInfo.depthStencilInfo.back = {};  // Optional

    pipelineConfigInfo.dynamicStateEnables = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    pipelineConfigInfo.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipelineConfigInfo.dynamicStateInfo.pDynamicStates = pipelineConfigInfo.dynamicStateEnables.data();
    pipelineConfigInfo.dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(pipelineConfigInfo.dynamicStateEnables.size());
    pipelineConfigInfo.dynamicStateInfo.flags = 0;


    return pipelineConfigInfo;
}

void CreateGraphicsPipeline(ZaynMemory *zaynMem, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath, PipelineConfigInfo *myDefaultPipelineConfigInfo)
{

    auto vertShaderCode = readFile(vertShaderFilePath);
    auto fragShaderCode = readFile(fragShaderFilePath);

    zaynMem->vkVertShaderModule = createShaderModule(vertShaderCode, zaynMem);
    zaynMem->vkFragShaderModule = createShaderModule(fragShaderCode, zaynMem);

    // zaynMem->MyPipelineConfigInfo = MakeMyDefaultPipelineConfig(zaynMem, zaynMem->window.size.x, zaynMem->window.size.y);

    assert(
        zaynMem->MyPipelineConfigInfo.pipelineLayout != nullptr &&
        "Cannot create graphics pipeline: no pipelineLayout provided in config info");
    assert(
        zaynMem->MyPipelineConfigInfo.renderPass != nullptr &&
        "Cannot create graphics pipeline: no renderPass provided in config info");

    VkPipelineShaderStageCreateInfo shaderStages[2];
    shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    shaderStages[0].module = zaynMem->vkVertShaderModule;
    shaderStages[0].pName = "main";
    shaderStages[0].flags = 0;
    shaderStages[0].pNext = nullptr;
    shaderStages[0].pSpecializationInfo = nullptr;
    shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    shaderStages[1].module = zaynMem->vkFragShaderModule;
    shaderStages[1].pName = "main";
    shaderStages[1].flags = 0;
    shaderStages[1].pNext = nullptr;
    shaderStages[1].pSpecializationInfo = nullptr;

    auto bindingDescriptions = getBindingDescriptions();
    auto attributeDescriptions = getAttributeDescriptions();

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescriptions.size());
    
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    
    vertexInputInfo.pVertexBindingDescriptions = bindingDescriptions.data();     // Optional
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

    VkPipelineViewportStateCreateInfo viewportStateInfo = {};
    viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportStateInfo.viewportCount = 1;
    // viewportStateInfo.pViewports = &myDefaultPipelineConfigInfo->viewport;
    viewportStateInfo.pViewports = nullptr;
    viewportStateInfo.scissorCount = 1;
    // viewportStateInfo.pScissors = &myDefaultPipelineConfigInfo->scissor;
    viewportStateInfo.pScissors = nullptr;

    VkPipelineColorBlendAttachmentState colorBlendAttachmentsInfo = {};
    colorBlendAttachmentsInfo.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachmentsInfo.blendEnable = VK_FALSE;
    colorBlendAttachmentsInfo.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;                // Optional
    colorBlendAttachmentsInfo.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;               // Optional
    colorBlendAttachmentsInfo.colorBlendOp = VK_BLEND_OP_ADD;                           // Optional
    colorBlendAttachmentsInfo.srcAlphaBlendFactor = VkBlendFactor::VK_BLEND_FACTOR_ONE; // Optional
    colorBlendAttachmentsInfo.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;               // Optional
    colorBlendAttachmentsInfo.alphaBlendOp = VK_BLEND_OP_ADD;                           // Optional

    VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
    colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlendInfo.logicOpEnable = VK_FALSE;
    colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlendInfo.attachmentCount = 1;
    colorBlendInfo.pAttachments = &colorBlendAttachmentsInfo;
    colorBlendInfo.blendConstants[0] = 0.0f; // Optional
    colorBlendInfo.blendConstants[1] = 0.0f; // Optional
    colorBlendInfo.blendConstants[2] = 0.0f; // Optional
    colorBlendInfo.blendConstants[3] = 0.0f; // Optional

    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &myDefaultPipelineConfigInfo->inputAssemblyInfo;
    pipelineInfo.pViewportState = &viewportStateInfo;
    pipelineInfo.pRasterizationState = &myDefaultPipelineConfigInfo->rasterizationInfo;
    pipelineInfo.pMultisampleState = &myDefaultPipelineConfigInfo->multisampleInfo;
    pipelineInfo.pColorBlendState = &colorBlendInfo;
    pipelineInfo.pDynamicState = &myDefaultPipelineConfigInfo->dynamicStateInfo; // Optional
    pipelineInfo.pDepthStencilState = &myDefaultPipelineConfigInfo->depthStencilInfo;

    pipelineInfo.layout = myDefaultPipelineConfigInfo->pipelineLayout;
    pipelineInfo.renderPass = myDefaultPipelineConfigInfo->renderPass;
    // pipelineInfo.renderPass = myDefaultPipelineConfigInfo->renderPass;
    pipelineInfo.subpass = myDefaultPipelineConfigInfo->subpass;

    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
    pipelineInfo.basePipelineIndex = -1;

    if (vkCreateGraphicsPipelines(zaynMem->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &zaynMem->vkGraphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }
}

void CreateCommandPool(ZaynMemory *zaynMem)
{
    QueueFamilyIndices queueFamilyIndices = findQueueFamilies(zaynMem->vkPhysicalDevice, zaynMem);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(zaynMem->vkDevice, &poolInfo, nullptr, &zaynMem->vkCommandPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create command pool!");
    }
}

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &availableFormats)
{
    for (const auto &availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &availablePresentModes)
{
    for (const auto &availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            std::cout << "Present mode: Mailbox" << std::endl;
            return availablePresentMode;
        }
    }
    std::cout << "Present mode: V-Sync" << std::endl;
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, ZaynMemory *zaynMem)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfwGetFramebufferSize(zaynMem->window, &width, &height);

        VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

void CreateSwapChain(ZaynMemory *zaynMem)
{
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(zaynMem->vkPhysicalDevice, zaynMem);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, zaynMem);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
    {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = zaynMem->vkSurface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = findQueueFamilies(zaynMem->vkPhysicalDevice, zaynMem);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

    if (indices.graphicsFamily != indices.presentFamily)
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else
    {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;     // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;


    VkSwapchainKHR oldSwapChain = zaynMem->vkSwapChain; 

    if (oldSwapChain == VK_NULL_HANDLE) 
    {
        createInfo.oldSwapchain = VK_NULL_HANDLE; // No existing swapchain
    } 
    else 
    {
        createInfo.oldSwapchain = oldSwapChain; // Use the existing swapchain
    }


    if (vkCreateSwapchainKHR(zaynMem->vkDevice, &createInfo, nullptr, &zaynMem->vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

    // we only specified a minimum number of images in the swap chain, so the implementation is
    // allowed to create a swap chain with more. That's why we'll first query the final number of
    // images with vkGetSwapchainImagesKHR, then resize the container and finally call it again to
    // retrieve the handles.
    vkGetSwapchainImagesKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, &imageCount, nullptr);
    zaynMem->vkSwapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, &imageCount, zaynMem->vkSwapChainImages.data());

    zaynMem->vkSwapChainImageFormat = surfaceFormat.format;
    zaynMem->vkSwapChainExtent = extent;
}

void CreateImageViews(ZaynMemory *zaynMem)
{
    zaynMem->vkSwapChainImageViews.resize(zaynMem->vkSwapChainImages.size());
    for (size_t i = 0; i < zaynMem->vkSwapChainImages.size(); i++)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = zaynMem->vkSwapChainImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = zaynMem->vkSwapChainImageFormat;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(zaynMem->vkDevice, &viewInfo, nullptr, &zaynMem->vkSwapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }
    }
}

VkFormat findSupportedFormat(const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features, ZaynMemory *zaynMem)
{
    for (VkFormat format : candidates)
    {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(zaynMem->vkPhysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
        {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
        {
            return format;
        }
    }
    throw std::runtime_error("failed to find supported format!");
}

VkFormat findDepthFormat(ZaynMemory *zaynMem)
{
    return findSupportedFormat(
        {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT,
        zaynMem);
}

void CreateRenderPass(ZaynMemory *zaynMem)
{
    VkAttachmentDescription depthAttachment = {};
    depthAttachment.format = findDepthFormat(zaynMem);
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef = {};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = zaynMem->vkSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef = {};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.srcAccessMask = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstSubpass = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 2> attachments = {colorAttachment, depthAttachment};
    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(zaynMem->vkDevice, &renderPassInfo, nullptr, &zaynMem->vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void createImageWithInfo(const VkImageCreateInfo &imageInfo, VkMemoryPropertyFlags properties, VkImage &image, VkDeviceMemory &imageMemory, ZaynMemory *zaynMem)
{
    if (vkCreateImage(zaynMem->vkDevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(zaynMem->vkDevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, zaynMem);

    if (vkAllocateMemory(zaynMem->vkDevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }

    if (vkBindImageMemory(zaynMem->vkDevice, image, imageMemory, 0) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to bind image memory!");
    }
}

void CreateDepthResources(ZaynMemory *zaynMem)
{
    VkFormat depthFormat = findDepthFormat(zaynMem);
    VkExtent2D swapChainExtent = zaynMem->vkSwapChainExtent;

    zaynMem->vkDepthImages.resize(zaynMem->vkSwapChainImages.size());
    zaynMem->vkDepthImageMemorys.resize(zaynMem->vkSwapChainImages.size());
    zaynMem->vkDepthImageViews.resize(zaynMem->vkSwapChainImages.size());

    for (int i = 0; i < zaynMem->vkDepthImages.size(); i++)
    {
        VkImageCreateInfo imageInfo = {};
        imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        imageInfo.imageType = VK_IMAGE_TYPE_2D;
        imageInfo.extent.width = zaynMem->vkSwapChainExtent.width;
        imageInfo.extent.height = zaynMem->vkSwapChainExtent.height;
        imageInfo.extent.depth = 1;
        imageInfo.mipLevels = 1;
        imageInfo.arrayLayers = 1;
        imageInfo.format = depthFormat;
        imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
        imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        imageInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
        imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
        imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
        imageInfo.flags = 0;

        createImageWithInfo(imageInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, zaynMem->vkDepthImages[i], zaynMem->vkDepthImageMemorys[i], zaynMem);

        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = zaynMem->vkDepthImages[i];
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = depthFormat;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(zaynMem->vkDevice, &viewInfo, nullptr, &zaynMem->vkDepthImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create texture image view!");
        }
    }
}

void CreateFrameBuffers(ZaynMemory *zaynMem)
{
    zaynMem->vkSwapChainFramebuffers.resize(zaynMem->vkSwapChainImages.size());
    for (size_t i = 0; i < zaynMem->vkSwapChainImages.size(); i++)
    {
        std::array<VkImageView, 2> attachments = {zaynMem->vkSwapChainImageViews[i], zaynMem->vkDepthImageViews[i]};

        VkExtent2D swapChainExtent = zaynMem->vkSwapChainExtent;
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = zaynMem->vkRenderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = zaynMem->vkSwapChainExtent.width;
        framebufferInfo.height = zaynMem->vkSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(
                zaynMem->vkDevice,
                &framebufferInfo,
                nullptr,
                &zaynMem->vkSwapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void CreateSyncObjects(ZaynMemory *zaynMem)
{
    zaynMem->vkImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkImagesInFlight.resize(zaynMem->vkSwapChainImages.size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(zaynMem->vkDevice, &semaphoreInfo, nullptr, &zaynMem->vkImageAvailableSemaphores[i]) !=
                VK_SUCCESS ||
            vkCreateSemaphore(zaynMem->vkDevice, &semaphoreInfo, nullptr, &zaynMem->vkRenderFinishedSemaphores[i]) !=
                VK_SUCCESS ||
            vkCreateFence(zaynMem->vkDevice, &fenceInfo, nullptr, &zaynMem->vkInFlightFences[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

void MySwapChainCreation(ZaynMemory *zaynMem)
{

    CreateSwapChain(zaynMem);
    CreateImageViews(zaynMem);
    CreateRenderPass(zaynMem);
    CreateDepthResources(zaynMem);
    CreateFrameBuffers(zaynMem);
    CreateSyncObjects(zaynMem);
}

void VulkanInitDevice(ZaynMemory *zaynMem)
{
    CreateVKInstance(zaynMem);    // is same
    SetupDebugMessenger(zaynMem); // is SAME
    CreateSurface(zaynMem);       // is SAME
    PickPhysicalDevice(zaynMem);  // is SAME  // <-- @TODO: May need to modify to account for other machines like rPi
    CreateLogicalDevice(zaynMem); // is SAME
    CreateCommandPool(zaynMem);   // is SAME
}

void CreatePipelineLayout(ZaynMemory *zaynMem)
{
    VkPushConstantRange pushConstantRange = {};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(PushConstantData_2D);


    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    if (vkCreatePipelineLayout(zaynMem->vkDevice, &pipelineLayoutInfo, nullptr, &zaynMem->vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}

void CreatePipeline(ZaynMemory *zaynMem)
{
    zaynMem->MyPipelineConfigInfo = MakeMyDefaultPipelineConfig(zaynMem, zaynMem->vkSwapChainExtent.width, zaynMem->vkSwapChainExtent.height);
    zaynMem->MyPipelineConfigInfo.renderPass = zaynMem->vkRenderPass;
    zaynMem->MyPipelineConfigInfo.pipelineLayout = zaynMem->vkPipelineLayout;
}


void FreeCommandBuffers(ZaynMemory* zaynMem)
{
    vkFreeCommandBuffers(zaynMem->vkDevice, zaynMem->vkCommandPool, static_cast<uint32_t>(zaynMem->vkCommandBuffers.size()), zaynMem->vkCommandBuffers.data());
    zaynMem->vkCommandBuffers.clear();
}


void CreateCommandBuffers(ZaynMemory *zaynMem)
{
    zaynMem->vkCommandBuffers.resize(zaynMem->vkSwapChainImages.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = zaynMem->vkCommandPool;
    allocInfo.commandBufferCount = static_cast<uint32_t>(zaynMem->vkCommandBuffers.size());

    if (vkAllocateCommandBuffers(zaynMem->vkDevice, &allocInfo, zaynMem->vkCommandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    // for (int i = 0; i < zaynMem->vkCommandBuffers.size(); i++)
    // {
        
    // }
}

void RecreateSwapChain(ZaynMemory* zaynMem)
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(zaynMem->window, &width, &height);
    while (width == 0 || height == 0)
    {
        // std::cout << "MIN" << std::endl;
        glfwGetFramebufferSize(zaynMem->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(zaynMem->vkDevice);

    CreateSwapChain(zaynMem);

    if (zaynMem->vkSwapChain != nullptr)
    {
        if (zaynMem->vkSwapChainImages.size() != zaynMem->vkCommandBuffers.size()) 
        {
            FreeCommandBuffers(zaynMem);
            CreateCommandBuffers(zaynMem);
        }
    }
   
    CreatePipeline(zaynMem);

}


void bindCommandBufferToPipeline(VkCommandBuffer commandBuffer)
{
}


VkResult AcquireNextImage(uint32_t *imageIndex, ZaynMemory *zaynMem)
{
    vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());

    VkResult result = vkAcquireNextImageKHR(
        zaynMem->vkDevice,
        zaynMem->vkSwapChain,
        std::numeric_limits<uint64_t>::max(),
        zaynMem->vkImageAvailableSemaphores[zaynMem->vkCurrentFrame], // must be a not signaled semaphore
        VK_NULL_HANDLE,
        imageIndex);

    return result;
}



void RenderEntity(ZaynMemory *zaynMem, VkCommandBuffer imageBuffer,  Entity* entity)
{
    vkCmdBindPipeline(imageBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, zaynMem->vkGraphicsPipeline);

    for (int i = 0; i < 4; i++)
    {
        PushConstantData pushData;
        pushData.offset = entity->transform2d.translation;
        pushData.color = entity->color;
        // pushData.scale = entity->transform2d.scale;
        real32 cos_ = sinf(entity->transform2d.rotation * i);
        real32 sin_ = cosf(entity->transform2d.rotation * i);

        mat2 rotMatrix =   {cos_, sin_,-sin_, cos_};

        pushData.transform = rotMatrix * Scale2(entity->transform2d.scale);
        vkCmdPushConstants(imageBuffer, zaynMem->vkPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &pushData);
        BindModel(imageBuffer, &zaynMem->model1);
        DrawModel(imageBuffer, &zaynMem->model1);
    }
}

void RecordCommandBuffer(int32 ImageIndex, ZaynMemory* zaynMem)
{
    // VkCommandBufferBeginInfo beginInfo{};
    // beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    // if (vkBeginCommandBuffer(zaynMem->vkCommandBuffers[ImageIndex], &beginInfo) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to begin recording command buffer!");
    // }

    // VkRenderPassBeginInfo renderPassInfo{};
    // renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    // renderPassInfo.renderPass = zaynMem->vkRenderPass;
    // renderPassInfo.framebuffer = zaynMem->vkSwapChainFramebuffers[ImageIndex];

    // renderPassInfo.renderArea.offset = {0, 0};
    // renderPassInfo.renderArea.extent = zaynMem->vkSwapChainExtent;

    // std::array<VkClearValue, 2> clearValues{};
    // clearValues[0].color = {0.03f, 0.03f, 0.03f, 1.0f};
    // clearValues[1].depthStencil = {1.0f, 0};
    // renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    // renderPassInfo.pClearValues = clearValues.data();

    // vkCmdBeginRenderPass(zaynMem->vkCommandBuffers[ImageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    // VkViewport viewport{};
    // viewport.x = 0.0f;
    // viewport.y = 0.0f;
    // viewport.width = static_cast<float>(zaynMem->vkSwapChainExtent.width);
    // viewport.height = static_cast<float>(zaynMem->vkSwapChainExtent.height);
    // viewport.minDepth = 0.0f;
    // viewport.maxDepth = 1.0f;
    // VkRect2D scissor{{0, 0}, zaynMem->vkSwapChainExtent};
    // vkCmdSetViewport(zaynMem->vkCommandBuffers[ImageIndex], 0, 1, &viewport);
    // vkCmdSetScissor(zaynMem->vkCommandBuffers[ImageIndex], 0, 1, &scissor);

    vkCmdBindPipeline(zaynMem->vkCommandBuffers[ImageIndex], VK_PIPELINE_BIND_POINT_GRAPHICS, zaynMem->vkGraphicsPipeline);
    BindModel(zaynMem->vkCommandBuffers[ImageIndex], &zaynMem->model1);

    // for (int i = 0; i < 4; i++)
    // {
    //     PushConstantData pushData;
    //     pushData.offset = {-0.3f + i * 0.1f, -0.7f + i * 0.2f};
    //     pushData.color = {0.03f + i * 0.1f, 0.4f + i * 0.2f, 0.8f - i * 0.2f};
    //     vkCmdPushConstants(zaynMem->vkCommandBuffers[ImageIndex], zaynMem->vkPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(PushConstantData), &pushData);
    //     DrawModel(zaynMem->vkCommandBuffers[ImageIndex], &zaynMem->model1);
    // }

    // Monkey* testMonkey = GetEntity(&Casette->em, Monkey, zaynMem->monkeyHandle1);
    // testMonkey->transform2d.rotation += 0.009f;
    // RenderEntity(zaynMem, zaynMem->vkCommandBuffers[ImageIndex], testMonkey);



    // if (vkEndCommandBuffer(zaynMem->vkCommandBuffers[ImageIndex]) != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to record command buffer!");
    // }
}

bool BeginFrame(ZaynMemory *zaynMem)
{
    assert(!zaynMem->vkIsFrameStarted && "cannot call begin frame when frame buffer is already in progress");

    // uint32_t imageIndex;
    auto result = AcquireNextImage(&zaynMem->vkCurrentImageIndex, zaynMem);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain(zaynMem);
        return true;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    zaynMem->vkIsFrameStarted = true;

    // zaynMem->vkNextCommandBuffer = zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame];

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
        return false;
    }

    return true;
}

VkResult SubmitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex, ZaynMemory *zaynMem)
{
    if (zaynMem->vkImagesInFlight[*imageIndex] != VK_NULL_HANDLE)
    {
        vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkImagesInFlight[*imageIndex], VK_TRUE, UINT64_MAX);
    }
    zaynMem->vkImagesInFlight[*imageIndex] = zaynMem->vkImagesInFlight[zaynMem->vkCurrentFrame];

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {zaynMem->vkImageAvailableSemaphores[zaynMem->vkCurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = buffers;

    VkSemaphore signalSemaphores[] = {zaynMem->vkRenderFinishedSemaphores[zaynMem->vkCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(zaynMem->vkDevice, 1, &zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame]);
    if (vkQueueSubmit(zaynMem->vkGraphicsQueue, 1, &submitInfo, zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {zaynMem->vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = imageIndex;

    auto result = vkQueuePresentKHR(zaynMem->vkPresentQueue, &presentInfo);

    zaynMem->vkCurrentFrame = (zaynMem->vkCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    return result;
}

void EndFrame(ZaynMemory *zaynMem)
{
    assert(zaynMem->vkIsFrameStarted && "Can't call endFrame while frame is not in progress");
    auto _commandBuffer = zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame];

    if (vkEndCommandBuffer(_commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
    
    auto result = SubmitCommandBuffers(&_commandBuffer, &zaynMem->vkCurrentImageIndex, zaynMem);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || zaynMem->vkFramebufferResized)
    {
        zaynMem->vkFramebufferResized = false;
        RecreateSwapChain(zaynMem);
    }
    if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    zaynMem->vkIsFrameStarted = false;

    zaynMem->vkCurrentFrame = (zaynMem->vkCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

}


void BeginSwapChainRenderPass(ZaynMemory* zaynMem, VkCommandBuffer commandBuffer)
{
    assert(zaynMem->vkIsFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame] && "Can't begin render pass on command buffer from a different frame");

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = zaynMem->vkRenderPass;
    renderPassInfo.framebuffer = zaynMem->vkSwapChainFramebuffers[zaynMem->vkCurrentImageIndex];

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = zaynMem->vkSwapChainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.03f, 0.03f, 0.03f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(zaynMem->vkSwapChainExtent.width);
    viewport.height = static_cast<float>(zaynMem->vkSwapChainExtent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, zaynMem->vkSwapChainExtent};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

}
void EndSwapChainRenderPass(ZaynMemory* zaynMem, VkCommandBuffer commandBuffer)
{
    assert(zaynMem->vkIsFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
    assert(commandBuffer == zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame] && "Can't begin render pass on command buffer from a different frame");

    vkCmdEndRenderPass(commandBuffer);

}


void DrawFrame(ZaynMemory *zaynMem)
{
    // uint32_t imageIndex;
    // auto result = AcquireNextImage(&imageIndex, zaynMem);

    // if (result == VK_ERROR_OUT_OF_DATE_KHR)
    // {
    //     RecreateSwapChain(zaynMem);
    // }

    // if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    // {
    //     throw std::runtime_error("failed to acquire swap chain image!");
    // }

    // RecordCommandBuffer(imageIndex, zaynMem);

    // result = SubmitCommandBuffers(&zaynMem->vkCommandBuffers[imageIndex], &imageIndex, zaynMem);

    // if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || zaynMem->vkFramebufferResized)
    // {
    //     zaynMem->vkFramebufferResized = false;
    //     RecreateSwapChain(zaynMem);
    //     return;
    // }
    // if (result != VK_SUCCESS)
    // {
    //     throw std::runtime_error("failed to present swap chain image!");
    // }
}



void InitRender_Learn(ZaynMemory *zaynMem)
{

    VulkanInitDevice(zaynMem); // iTHis is the contstructor code for the online video

    MySwapChainCreation(zaynMem);

    CreatePipelineLayout(zaynMem);

    std::vector<Vertex> vertices = {
        {{0.0f, -0.5f}, {0.4f, 0.6f, 0.2f}},
        {{
             0.5f,
             0.5f,
         },
         {0.1f, 0.1f, 0.1f}},
        {{-0.1f, 0.5f}, {0.9f, 0.3f, 0.9f}}};

    // std::vector<Vertex> vertices = {};
    // sierpinski(vertices, 3, {-1.0f, 1.0f}, {1.0f, 1.0f}, {0.0f, -1.0f});
    ModelInit(&zaynMem->vkDevice, vertices, &zaynMem->model1, zaynMem);

    CreatePipeline(zaynMem);
    // RecreateSwapChain(zaynMem);
    CreateGraphicsPipeline(zaynMem, "/Users/socki/dev/zayn/src/renderer/shaders/vkShader_03_vert.spv", "/Users/socki/dev/zayn/src/renderer/shaders/vkShader_03_frag.spv", &zaynMem->MyPipelineConfigInfo);

    CreateCommandBuffers(zaynMem);

    VkPhysicalDeviceProperties deviceProperties; // Create a struct to hold properties
    vkGetPhysicalDeviceProperties(zaynMem->vkPhysicalDevice, &deviceProperties);

    // Now you can access the properties:
    std::cout << "Max Push Constant Size: "
              << deviceProperties.limits.maxPushConstantsSize
              << std::endl; // Model model;
}




void UpdateRender_Learn(ZaynMemory *zaynMem)
{
    // DrawFrame(zaynMem);

    if (BeginFrame(zaynMem) )
    {
        BeginSwapChainRenderPass(zaynMem, zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame]);

        Monkey *testMonkey = GetEntity(&Casette->em, Monkey, zaynMem->monkeyHandle1);
        testMonkey->transform2d.rotation += 0.009f;

        RenderEntity(zaynMem, zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame], testMonkey);

        EndSwapChainRenderPass(zaynMem, zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame]);
        EndFrame(zaynMem);
    }
    
}

void RenderCleanup(ZaynMemory *zaynMem)
{

    vkDestroyShaderModule(zaynMem->vkDevice, zaynMem->vkVertShaderModule, nullptr);
    vkDestroyShaderModule(zaynMem->vkDevice, zaynMem->vkFragShaderModule, nullptr);
    vkDestroyPipeline(zaynMem->vkDevice, zaynMem->vkGraphicsPipeline, nullptr);
    vkDestroyDevice(zaynMem->vkDevice, nullptr);
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(zaynMem->vkInstance, zaynMem->vkDebugMessenger, nullptr);
    }

    vkDeviceWaitIdle(zaynMem->vkDevice);
}