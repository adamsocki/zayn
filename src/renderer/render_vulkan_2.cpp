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



const std::vector<Vertex> vertices =
    {
        {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};

struct UniformBufferObject
{
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
  alignas(16) bool boolHello;

};

struct UniformBufferObject2
{
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

const std::vector<uint16_t> indices =
    {
        0, 1, 2, 2, 3, 0};

// VkVertexInputBindingDescription getBindingDescription()
// {
//     VkVertexInputBindingDescription bindingDescription{};
//     bindingDescription.binding = 0;
//     bindingDescription.stride = sizeof(Vertex);
//     bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//     return bindingDescription;
// }

// std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions()
// {
//     std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
//     attributeDescriptions[0].binding = 0;
//     attributeDescriptions[0].location = 0;
//     attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
//     attributeDescriptions[0].offset = offsetof(Vertex, position);

//     attributeDescriptions[1].binding = 0;
//     attributeDescriptions[1].location = 1;
//     attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//     attributeDescriptions[1].offset = offsetof(Vertex, color);

//     return attributeDescriptions;
// }

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
    VkLayerProperties *availableLayers = (VkLayerProperties*)malloc(layerCount * sizeof(VkLayerProperties));

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
    createInfo.pUserData = nullptr;  // Optional
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

    if (CreateDebugUtilsMessengerEXT(zaynMem->vkInstance, &createInfo, &pAllocator, &zaynMem->vkDebugMessenger) != VK_SUCCESS)
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
            return availablePresentMode;
        }
    }

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

        VkExtent2D actualExtent =
            {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)};

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

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = zaynMem->vkSurface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;   // Use VK_IMAGE_USAGE_TRANSFER_DST_BIT for post-processing | use a memory operation to transfer the rendered image to a swap chain image.

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
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(zaynMem->vkDevice, &createInfo, nullptr, &zaynMem->vkSwapChain) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create swap chain!");
    }

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
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = zaynMem->vkSwapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = zaynMem->vkSwapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(zaynMem->vkDevice, &createInfo, nullptr, &zaynMem->vkSwapChainImageViews[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image views!");
        }
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
    createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(zaynMem->vkDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

void CreateRenderPass(ZaynMemory *zaynMem)
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = zaynMem->vkSwapChainImageFormat;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(zaynMem->vkDevice, &renderPassInfo, nullptr, &zaynMem->vkRenderPass) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create render pass!");
    }
}

void CreateGraphicsPipeline(ZaynMemory *zaynMem, const std::string &vertShaderFilePath, const std::string &fragShaderFilePath)
{
    auto vertShaderCode = readFile(vertShaderFilePath);
    auto fragShaderCode = readFile(fragShaderFilePath);

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode, zaynMem);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode, zaynMem);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions();

    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &zaynMem->vkDescriptorSetLayout;

    if (vkCreatePipelineLayout(zaynMem->vkDevice, &pipelineLayoutInfo, nullptr, &zaynMem->vkPipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = zaynMem->vkPipelineLayout;
    pipelineInfo.renderPass = zaynMem->vkRenderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (vkCreateGraphicsPipelines(zaynMem->vkDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &zaynMem->vkGraphicsPipeline) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    vkDestroyShaderModule(zaynMem->vkDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(zaynMem->vkDevice, vertShaderModule, nullptr);
}

void CreateFrameBuffers(ZaynMemory *zaynMem)
{
    zaynMem->vkSwapChainFramebuffers.resize(zaynMem->vkSwapChainImageViews.size());

    for (size_t i = 0; i < zaynMem->vkSwapChainImageViews.size(); i++)
    {
        VkImageView attachments[] =
            {
                zaynMem->vkSwapChainImageViews[i]};

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = zaynMem->vkRenderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = zaynMem->vkSwapChainExtent.width;
        framebufferInfo.height = zaynMem->vkSwapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(zaynMem->vkDevice, &framebufferInfo, nullptr, &zaynMem->vkSwapChainFramebuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create framebuffer!");
        }
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

void recordCommandBuffer(ZaynMemory *zaynMem, VkCommandBuffer commandBuffer, uint32_t imageIndex)
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = zaynMem->vkRenderPass;
    renderPassInfo.framebuffer = zaynMem->vkSwapChainFramebuffers[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = zaynMem->vkSwapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, zaynMem->vkGraphicsPipeline);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)zaynMem->vkSwapChainExtent.width;
    viewport.height = (float)zaynMem->vkSwapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = zaynMem->vkSwapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    VkBuffer vertexBuffers[] = {zaynMem->vkVertexBuffer};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, zaynMem->vkIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, zaynMem->vkPipelineLayout, 0, 1, &zaynMem->vkDescriptorSets[zaynMem->vkCurrentFrame], 0, nullptr);

    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(commandBuffer);

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void CreateCommandBuffers(ZaynMemory *zaynMem)
{
    zaynMem->vkCommandBuffers = (VkCommandBuffer *)malloc(sizeof(VkCommandBuffer) * MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = zaynMem->vkCommandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = MAX_FRAMES_IN_FLIGHT;

    if (vkAllocateCommandBuffers(zaynMem->vkDevice, &allocInfo, zaynMem->vkCommandBuffers) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void CreateSyncObjects(ZaynMemory *zaynMem)
{
    zaynMem->vkImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(zaynMem->vkDevice, &semaphoreInfo, nullptr, &zaynMem->vkImageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(zaynMem->vkDevice, &semaphoreInfo, nullptr, &zaynMem->vkRenderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(zaynMem->vkDevice, &fenceInfo, nullptr, &zaynMem->vkInFlightFences[i]) != VK_SUCCESS)
        {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, ZaynMemory *zaynMem)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(zaynMem->vkPhysicalDevice, &memProperties);
    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
    {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
        {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer &buffer, VkDeviceMemory &bufferMemory, ZaynMemory *zaynMem)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(zaynMem->vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(zaynMem->vkDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, zaynMem);

    if (vkAllocateMemory(zaynMem->vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(zaynMem->vkDevice, buffer, bufferMemory, 0);
}

void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, ZaynMemory *zaynMem)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = zaynMem->vkCommandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(zaynMem->vkDevice, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(zaynMem->vkGraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(zaynMem->vkGraphicsQueue);

    vkFreeCommandBuffers(zaynMem->vkDevice, zaynMem->vkCommandPool, 1, &commandBuffer);
}

void CreateVertexBuffer(ZaynMemory *zaynMem)
{
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // STAGING BUFFER - CPU accessible memory to upload the data from the vertex array to.
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, zaynMem);

    void *data;
    vkMapMemory(zaynMem->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(zaynMem->vkDevice, stagingBufferMemory);


    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, zaynMem->vkVertexBuffer, zaynMem->vkVertexBufferMemory, zaynMem);

    copyBuffer(stagingBuffer, zaynMem->vkVertexBuffer, bufferSize, zaynMem);

    vkDestroyBuffer(zaynMem->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, stagingBufferMemory, nullptr);
}

void CreateIndexBuffer(ZaynMemory *zaynMem)
{
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory, zaynMem);

    void *data;
    vkMapMemory(zaynMem->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(zaynMem->vkDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, zaynMem->vkIndexBuffer, zaynMem->vkIndexBufferMemory, zaynMem);

    copyBuffer(stagingBuffer, zaynMem->vkIndexBuffer, bufferSize, zaynMem);

    vkDestroyBuffer(zaynMem->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, stagingBufferMemory, nullptr);
}

void CreateDescriptorSetLayout(ZaynMemory *zaynMem)
{
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(zaynMem->vkDevice, &layoutInfo, nullptr, &zaynMem->vkDescriptorSetLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

void CreateUniformBuffers(ZaynMemory *zaynMem)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    zaynMem->vkUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, zaynMem->vkUniformBuffers[i], zaynMem->vkUniformBuffersMemory[i], zaynMem);
        vkMapMemory(zaynMem->vkDevice, zaynMem->vkUniformBuffersMemory[i], 0, bufferSize, 0, &zaynMem->vkUniformBuffersMapped[i]);
    }
}

void CreateDescriptorPool(ZaynMemory *zaynMem)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(zaynMem->vkDevice, &poolInfo, nullptr, &zaynMem->vkDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void CreateDescriptorSets(ZaynMemory *zaynMem)
{
    std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, zaynMem->vkDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = zaynMem->vkDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    zaynMem->vkDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(zaynMem->vkDevice, &allocInfo, zaynMem->vkDescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = zaynMem->vkUniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = zaynMem->vkDescriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr;       // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional

        vkUpdateDescriptorSets(zaynMem->vkDevice, 1, &descriptorWrite, 0, nullptr);
    }
}

void VulkanInitCode(ZaynMemory* zaynMem)
{
    CreateVKInstance(zaynMem);          // is same
    SetupDebugMessenger(zaynMem);       // is SAME
}

void InitRender_Learn(ZaynMemory *zaynMem)
{

    VulkanInitCode(zaynMem);            // is SAME
        
    CreateSurface(zaynMem);             // is SAME

    PickPhysicalDevice(zaynMem);        // is SAME  // <-- @TODO: May need to modify to account for other machines like rPi
    CreateLogicalDevice(zaynMem);       // is SAME
    CreateSwapChain(zaynMem);
    CreateImageViews(zaynMem);
    CreateRenderPass(zaynMem);
    CreateDescriptorSetLayout(zaynMem);
    // CreateGraphicsPipeline(zaynMem, "../src/renderer/shaders/vkShader_uniform_01_vert.spv", "../src/renderer/shaders/vkShader_uniform_01_frag.spv");
    CreateGraphicsPipeline(zaynMem, "/Users/socki/dev/zayn/src/renderer/shaders/vkShader_uniform_01_vert.spv", "/Users/socki/dev/zayn/src/renderer/shaders/vkShader_01_frag.spv");
    CreateFrameBuffers(zaynMem);

    CreateCommandPool(zaynMem);
    
    CreateVertexBuffer(zaynMem);
    CreateIndexBuffer(zaynMem);
    CreateUniformBuffers(zaynMem);
    CreateDescriptorPool(zaynMem);
    CreateDescriptorSets(zaynMem);

    CreateCommandBuffers(zaynMem);
    CreateSyncObjects(zaynMem);
}

void CleanUpSwapChain(ZaynMemory *zaynMem)
{
    for (size_t i = 0; i < zaynMem->vkSwapChainFramebuffers.size(); i++)
    {
        vkDestroyFramebuffer(zaynMem->vkDevice, zaynMem->vkSwapChainFramebuffers[i], nullptr);
    }

    for (size_t i = 0; i < zaynMem->vkSwapChainImageViews.size(); i++)
    {
        vkDestroyImageView(zaynMem->vkDevice, zaynMem->vkSwapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, nullptr);
}

void RecreateSwapChain(ZaynMemory *zaynMem)
{

    // std::cout << "RecreateSwapChain" << std::endl;

    int width = 0, height = 0;
    glfwGetFramebufferSize(zaynMem->window, &width, &height);
    while (width == 0 || height == 0)
    {
        // std::cout << "MIN" << std::endl;
        glfwGetFramebufferSize(zaynMem->window, &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(zaynMem->vkDevice);

    CleanUpSwapChain(zaynMem);

    CreateSwapChain(zaynMem);
    CreateImageViews(zaynMem);
    CreateFrameBuffers(zaynMem);
}

void UpdateUniformBuffer(uint32_t currentImage, ZaynMemory *zaynMem)
{
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), zaynMem->vkSwapChainExtent.width / (float)zaynMem->vkSwapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(zaynMem->vkUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void UpdateRender_Learn(ZaynMemory *zaynMem)
{
    vkWaitForFences(zaynMem->vkDevice, 1, &zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    // NEW
    VkResult result = vkAcquireNextImageKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, UINT64_MAX, zaynMem->vkImageAvailableSemaphores[zaynMem->vkCurrentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        RecreateSwapChain(zaynMem);
        return;
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    UpdateUniformBuffer(zaynMem->vkCurrentFrame, zaynMem);

    // IS THIS WHERE ALL UPDATES CAN GO?

    // END NEW

    vkResetFences(zaynMem->vkDevice, 1, &zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame]);

    vkResetCommandBuffer(zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame], /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(zaynMem, zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame], imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {zaynMem->vkImageAvailableSemaphores[zaynMem->vkCurrentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &zaynMem->vkCommandBuffers[zaynMem->vkCurrentFrame];

    VkSemaphore signalSemaphores[] = {zaynMem->vkRenderFinishedSemaphores[zaynMem->vkCurrentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(zaynMem->vkGraphicsQueue, 1, &submitInfo, zaynMem->vkInFlightFences[zaynMem->vkCurrentFrame]) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {zaynMem->vkSwapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    // vkQueuePresentKHR(zaynMem->vkPresentQueue, &presentInfo);

    result = vkQueuePresentKHR(zaynMem->vkPresentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || zaynMem->vkFramebufferResized)
    {
        zaynMem->vkFramebufferResized = false;

        RecreateSwapChain(zaynMem);
    }
    else if (result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }

    zaynMem->vkCurrentFrame = (zaynMem->vkCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void RenderCleanup(ZaynMemory *zaynMem)
{

    CleanUpSwapChain(zaynMem);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkUniformBuffers[i], nullptr);
        vkFreeMemory(zaynMem->vkDevice, zaynMem->vkUniformBuffersMemory[i], nullptr);
    }

    vkDestroyDescriptorPool(zaynMem->vkDevice, zaynMem->vkDescriptorPool, nullptr);
    vkDestroyDescriptorSetLayout(zaynMem->vkDevice, zaynMem->vkDescriptorSetLayout, nullptr);

    vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkVertexBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkVertexBufferMemory, nullptr);

    vkDestroyBuffer(zaynMem->vkDevice, zaynMem->vkIndexBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, zaynMem->vkIndexBufferMemory, nullptr);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(zaynMem->vkDevice, zaynMem->vkRenderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(zaynMem->vkDevice, zaynMem->vkImageAvailableSemaphores[i], nullptr);
        vkDestroyFence(zaynMem->vkDevice, zaynMem->vkInFlightFences[i], nullptr);
    }
    vkDestroyCommandPool(zaynMem->vkDevice, zaynMem->vkCommandPool, nullptr);
    for (auto framebuffer : zaynMem->vkSwapChainFramebuffers)
    {
        vkDestroyFramebuffer(zaynMem->vkDevice, framebuffer, nullptr);
    }
    vkDestroyPipeline(zaynMem->vkDevice, zaynMem->vkGraphicsPipeline, nullptr);
    vkDestroyPipelineLayout(zaynMem->vkDevice, zaynMem->vkPipelineLayout, nullptr);
    vkDestroyRenderPass(zaynMem->vkDevice, zaynMem->vkRenderPass, nullptr);
    for (auto imageView : zaynMem->vkSwapChainImageViews)
    {
        vkDestroyImageView(zaynMem->vkDevice, imageView, nullptr);
    }
    vkDestroySwapchainKHR(zaynMem->vkDevice, zaynMem->vkSwapChain, nullptr);
    vkDestroyDevice(zaynMem->vkDevice, nullptr);
    if (enableValidationLayers)
    {
        DestroyDebugUtilsMessengerEXT(zaynMem->vkInstance, zaynMem->vkDebugMessenger, nullptr);
    }
    vkDestroySurfaceKHR(zaynMem->vkInstance, zaynMem->vkSurface, nullptr);
    vkDestroyInstance(zaynMem->vkInstance, nullptr);

    vkDeviceWaitIdle(zaynMem->vkDevice);

    free(zaynMem->vkCommandBuffers);
}