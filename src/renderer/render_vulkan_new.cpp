// ERROR LOGGING

#include <cstring>

#include <unordered_map>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>



#ifdef _WIN32
	#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
	#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

static void _vkh_message_log(const char* message, const char* file, int32_t line);
#define MSG_LOG(m) _vkh_message_log(m, __FILENAME__, __LINE__)

static void _vkh_error_log(const char* message, const char* file, int32_t line);
#define ERROR_LOG(m) _vkh_error_log(m, __FILENAME__, __LINE__)


//----------------------------------------------------------------------------//

static void _vkh_message_log(const char* message, const char* file, int32_t line)
{
	printf("VKH MESSAGE in %s at line %i - \"%s\"\n\n", file, line, message);
}

static void _vkh_error_log(const char* message, const char* file, int32_t line)
{
	printf("VKH ERROR in %s at line %i - \"%s\"\n\n", file, line, message);
}

// namespace std
// {
//     template <>
//     struct hash<Vertex_NEW>
//     {
//         size_t operator()(Vertex_NEW const &vertex) const
//         {
//             size_t seed = 0;
//             hashCombine(seed, vertex.position, vertex.normal);
//             return seed;
//         }
//     };
// }

// namespace std {
//     template<> struct hash<Vertex_NEW> {
//         size_t operator()(Vertex_NEW const& vertex) const {
//             return(hash<vec3>()(vertex.pos) ^ (hash<vec2>()(vertex.texCoord) << 1));
//         }
//     };
// }

//----------------------------------------------------------------------------//

std::vector<glm::vec2> vertices_triangle = {
        glm::vec2(0.0f, -0.5f),
        glm::vec2(0.5f, 0.5f),
        glm::vec2(-0.5f, 0.5f),
    };

    // Example index data for the triangle
    std::vector<uint32_t> indices_triangle = { 0, 1, 2 };

#if VKH_VALIDATION_LAYERS
	#define REQUIRED_LAYER_COUNT 1
	const char* REQUIRED_LAYERS[REQUIRED_LAYER_COUNT] = {"VK_LAYER_KHRONOS_validation"};
#endif

#if __APPLE__
    #define REQUIRED_DEVICE_EXTENSION_COUNT 3
#else
    #define REQUIRED_DEVICE_EXTENSION_COUNT 2
#endif

const char* REQUIRED_DEVICE_EXTENSIONS[REQUIRED_DEVICE_EXTENSION_COUNT] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
#if __APPLE__
    "VK_KHR_portability_subset"
#endif
};

//----------------------------------------------------------------------------//


//////////////////////////////////////////////////////////// 
//			      _    _         _       _ _   
//			_   _| | _| |__     (_)_ __ (_) |_ 
//			 \ / / |/ / '_ \    | | '_ \| | __|
//			\ V /|   <| | | |   | | | | | | |_ 
//			 \_/ |_|\_\_| |_|___|_|_| |_|_|\__|
//			               |_____|       
/////////////////////////////////////////////////////////////      

vkh_bool_t _vkh_init_window(VKHinstance* inst)
{
	MSG_LOG("initlalizing Window...");

	inst->window = Zayn->window;
	if(!inst->window)
	{
		ERROR_LOG("failed to connect to glfw window");
		return VKH_FALSE;
	}

	return VKH_TRUE;
}
static VKAPI_ATTR VkBool32 _vkh_vk_debug_callback(
	VkDebugUtilsMessageSeverityFlagBitsEXT sevrerity,
	VkDebugUtilsMessageTypeFlagsEXT type,
	const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
	void* userData)
{
	if(sevrerity < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		return VK_FALSE;

	printf("VKH VALIDATION LAYER - %s\n\n", callbackData->pMessage);
	return VK_FALSE;
}


vkh_bool_t _vkh_create_vk_instance(VKHinstance* inst)
{
	MSG_LOG("creating Vulkan instance...");

	//get required GLFW extensions:
	//---------------
	uint32_t requiredExtensionCount;
	char** requiredGlfwExtensions = (char**)glfwGetRequiredInstanceExtensions(&requiredExtensionCount);
	if(!requiredGlfwExtensions)
	{
		ERROR_LOG("Vulkan rendering not supported on this machine");
		return VKH_FALSE;
	}
	vkh_bool_t freeExtensionList = VKH_FALSE;

	//reserve space for portability extension
    //---------------
#if __APPLE__
    requiredExtensionCount += 2;
#endif

    #if VKH_VALIDATION_LAYERS
    requiredExtensionCount++;
    #endif

    char** requiredExtensions = (char**)malloc(requiredExtensionCount * sizeof(char*));
    memcpy(requiredExtensions, requiredGlfwExtensions, requiredExtensionCount * sizeof(char*));

    //set portability extension
    //---------------
#if __APPLE__
    requiredExtensions[requiredExtensionCount - 1] = const_cast<char*>(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    requiredExtensions[requiredExtensionCount - 2] = const_cast<char*>("VK_KHR_get_physical_device_properties2");
    #if VKH_VALIDATION_LAYERS
    requiredExtensions[requiredExtensionCount - 3] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    #endif
#else
    requiredExtensions[requiredExtensionCount - 1] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
#endif

	//check if glfw extensions are supported:
	//---------------
	uint32_t extensionCount;
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
	VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
	vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensions);

	for(uint32_t i = 0; i < requiredExtensionCount; i++)
	{
		vkh_bool_t found = VKH_FALSE;
		for(uint32_t j = 0; j < extensionCount; j++)
			if(strcmp(requiredExtensions[i], extensions[j].extensionName) == 0)
			{
				found = VKH_TRUE;
				break;
			}
		
		if(!found)
		{
			ERROR_LOG("1 or more required GLFW extensions not supported");;
			return VKH_FALSE;
		}
	}

	free(extensions);

	//check if validation layers are supported:
	//---------------
	uint32_t requiredLayerCount = 0;
	const char** requiredLayers;
	
	#if VKH_VALIDATION_LAYERS
	{
		requiredLayerCount = REQUIRED_LAYER_COUNT;
		requiredLayers = REQUIRED_LAYERS;

		uint32_t supportedLayerCount;
		vkEnumerateInstanceLayerProperties(&supportedLayerCount, NULL);
		VkLayerProperties* supportedLayers = (VkLayerProperties*)malloc(supportedLayerCount * sizeof(VkLayerProperties));
		vkEnumerateInstanceLayerProperties(&supportedLayerCount, supportedLayers);

		vkh_bool_t found = VKH_FALSE;
		for(uint32_t i = 0; i < requiredLayerCount; i++)
		{
			vkh_bool_t found = VKH_FALSE;
			for(uint32_t j = 0; j < supportedLayerCount; j++)
				if(strcmp(requiredLayers[i], supportedLayers[j].layerName) == 0)
				{
					found = VKH_TRUE;
					break;
				}

			if(!found)
			{
				ERROR_LOG("1 or more required validation layers not supported");
				return VKH_FALSE;
			}
		}

		free(supportedLayers);
	}
	#endif

	//create instance creation info structs:
	//---------------
	VkApplicationInfo appInfo = {}; //most of this stuff is pretty useless, just for drivers to optimize certain programs
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Zayn";
	appInfo.applicationVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.pEngineName = "Zayn Engine";
	appInfo.engineVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	appInfo.apiVersion = VK_MAKE_API_VERSION(0, 1, 0, 0);
	
	VkInstanceCreateInfo instanceInfo = {};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pApplicationInfo = &appInfo;

#if __APPLE__
    instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#endif

	instanceInfo.enabledExtensionCount = requiredExtensionCount;
	instanceInfo.ppEnabledExtensionNames = (const char* const*)requiredExtensions;
	instanceInfo.enabledLayerCount = requiredLayerCount;
	instanceInfo.ppEnabledLayerNames = requiredLayers;

	#if VKH_VALIDATION_LAYERS //not in blocks as debugInfo is used later
		VkDebugUtilsMessengerCreateInfoEXT debugInfo = {};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugInfo.messageSeverity = 
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
		debugInfo.messageType = 
			VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT;
		debugInfo.pfnUserCallback = _vkh_vk_debug_callback;
		debugInfo.pUserData = inst;

		instanceInfo.pNext = &debugInfo;
	#endif

	if(vkCreateInstance(&instanceInfo, NULL, &inst->instance) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create Vulkan instance");
		return VKH_FALSE;
	}

	if(freeExtensionList)
		free(requiredExtensions);
	
	//create debug messenger:
	//---------------
	#if VKH_VALIDATION_LAYERS
	{
		PFN_vkCreateDebugUtilsMessengerEXT createDebugMessenger = 
			(PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(inst->instance, "vkCreateDebugUtilsMessengerEXT");
		
		if(!createDebugMessenger)
		{
			ERROR_LOG("could not find function \"vkCreateDebugUtilsMessengerEXT\"");
			return VKH_FALSE;
		}

		if(createDebugMessenger(inst->instance, &debugInfo, NULL, &inst->debugMessenger) != VK_SUCCESS)
		{
			ERROR_LOG("failed to create debug messenger");
			return VKH_FALSE;
		}
	}	
	#endif

	//create surface:
	//---------------
	if(glfwCreateWindowSurface(inst->instance, inst->window, NULL, &inst->surface) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create window surface");
		return VKH_FALSE;
	}

	return VKH_TRUE;
}

vkh_bool_t _vkh_pick_physical_device(VKHinstance* inst)
{
	MSG_LOG("picking physical device...");

	uint32_t deviceCount;
	vkEnumeratePhysicalDevices(inst->instance, &deviceCount, NULL);

	if(deviceCount == 0)
	{
		ERROR_LOG("failed to find a physical device that supports Vulkan");
		return VKH_FALSE;
	}

	VkPhysicalDevice* devices = (VkPhysicalDevice*)malloc(deviceCount * sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(inst->instance, &deviceCount, devices);

	int32_t maxScore = -1;
	for(uint32_t i = 0; i < deviceCount; i++)
	{
		int32_t score = 0;

		VkPhysicalDeviceProperties properties;
		VkPhysicalDeviceFeatures features;
		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceProperties      (devices[i], &properties);
		vkGetPhysicalDeviceFeatures        (devices[i], &features);
		vkGetPhysicalDeviceMemoryProperties(devices[i], &memProperties);

		//check if required queue families are supported:
		//---------------
		int32_t graphicsComputeFamilyIdx = -1;
		int32_t presentFamilyIdx = -1;

		uint32_t queueFamilyCount;
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, NULL);
		VkQueueFamilyProperties* queueFamilies = (VkQueueFamilyProperties*)malloc(queueFamilyCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies);

		for(uint32_t j = 0; j < queueFamilyCount; j++)
		{
			if((queueFamilies[j].queueFlags & VK_QUEUE_GRAPHICS_BIT) &&
               (queueFamilies[j].queueFlags & VK_QUEUE_COMPUTE_BIT)) //TODO: see if there is a way to determine most optimal queue families
				graphicsComputeFamilyIdx = j;
			
			VkBool32 presentSupport;
			vkGetPhysicalDeviceSurfaceSupportKHR(devices[i], j, inst->surface, &presentSupport);
			if(presentSupport)
				presentFamilyIdx = j;

			if(graphicsComputeFamilyIdx > 0 && presentFamilyIdx > 0)
				break;
		}

		free(queueFamilies);

		if(graphicsComputeFamilyIdx < 0 || presentFamilyIdx < 0)
			continue;

		//check if required extensions are supported:
		//---------------
		vkh_bool_t extensionsSupported = VKH_TRUE;

		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, NULL);
		VkExtensionProperties* extensions = (VkExtensionProperties*)malloc(extensionCount * sizeof(VkExtensionProperties));
		vkEnumerateDeviceExtensionProperties(devices[i], NULL, &extensionCount, extensions);

		for(uint32_t j = 0; j < REQUIRED_DEVICE_EXTENSION_COUNT; j++)
		{
			vkh_bool_t found = VKH_FALSE;
			for(uint32_t k = 0; k < extensionCount; k++)
				if(strcmp(REQUIRED_DEVICE_EXTENSIONS[j], extensions[k].extensionName) == 0)
				{
					found = VKH_TRUE;
					break;
				}

			if(!found)
			{
				extensionsSupported = VKH_FALSE;
				break;
			}
		}

		free(extensions);

		if(!extensionsSupported)
			continue;

		//check if swapchain is supported:
		//---------------
		uint32_t swapchainFormatCount, swapchainPresentModeCount;
		vkGetPhysicalDeviceSurfaceFormatsKHR     (devices[i], inst->surface, &swapchainFormatCount     , NULL);
		vkGetPhysicalDeviceSurfacePresentModesKHR(devices[i], inst->surface, &swapchainPresentModeCount, NULL);

		if(swapchainFormatCount == 0 || swapchainPresentModeCount == 0)
			continue;

		//check if anisotropy is supported:
		//---------------
		if(features.samplerAnisotropy == VK_FALSE)
			continue;

		//score device:
		//---------------

		//TODO: consider more features/properties of the device, currently we only consider whether or not it is discrete
		if(properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			score += 1000;

		if(score > maxScore)
		{
			inst->physicalDevice = devices[i];
			inst->graphicsComputeFamilyIdx = graphicsComputeFamilyIdx;
			inst->presentFamilyIdx = presentFamilyIdx;

			maxScore = score;
		}
	}

	if(maxScore < 0)
	{
		ERROR_LOG("failed to find a suitable physical device");
		return VKH_FALSE;
	}

	free(devices);

	return VKH_TRUE;
}

vkh_bool_t _vkh_create_device(VKHinstance* inst)
{
	MSG_LOG("creating Vulkan device...");

	//create queue infos:
	//---------------

	//TODO: extend this to work with more than just 2 queue types
	//TODO: allow user to define which queues they would like, instead of just getting graphics and present
	uint32_t queueCount = 0;
	uint32_t queueIndices[2];
	if(inst->graphicsComputeFamilyIdx == inst->presentFamilyIdx)
	{
		queueCount = 1;
		queueIndices[0] = inst->graphicsComputeFamilyIdx;
	}
	else
	{
		queueCount = 2;
		queueIndices[0] = inst->graphicsComputeFamilyIdx;
		queueIndices[1] = inst->presentFamilyIdx;
	}

	float priority = 1.0f;
	VkDeviceQueueCreateInfo queueInfos[2];
	for(uint32_t i = 0; i < queueCount; i++)
	{
		VkDeviceQueueCreateInfo queueInfo = {};
		queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueInfo.queueFamilyIndex = queueIndices[i]; //TODO: see how we can optimize this, when would we want multiple queues?
		queueInfo.queueCount = 1;
		queueInfo.pQueuePriorities = &priority;

		queueInfos[i] = queueInfo;
	}

	//set features:
	//---------------
	VkPhysicalDeviceFeatures features = {}; //TODO: allow wanted features to be passed in
	features.samplerAnisotropy = VK_TRUE;

	//create device:
	//---------------
	VkDeviceCreateInfo deviceInfo = {};
	deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceInfo.queueCreateInfoCount = queueCount;
	deviceInfo.pQueueCreateInfos = queueInfos;
	deviceInfo.pEnabledFeatures = &features;
	deviceInfo.enabledExtensionCount = REQUIRED_DEVICE_EXTENSION_COUNT;
	deviceInfo.ppEnabledExtensionNames = REQUIRED_DEVICE_EXTENSIONS;
	#if VKH_VALIDATION_LAYERS
	{
		deviceInfo.enabledLayerCount = REQUIRED_LAYER_COUNT;
		deviceInfo.ppEnabledLayerNames = REQUIRED_LAYERS;
	}
	#endif

	if(vkCreateDevice(inst->physicalDevice, &deviceInfo, NULL, &inst->device) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create Vulkan device");
		return VKH_FALSE;
	}

	vkGetDeviceQueue(inst->device, inst->graphicsComputeFamilyIdx, 0, &inst->graphicsQueue);
	vkGetDeviceQueue(inst->device, inst->graphicsComputeFamilyIdx, 0, &inst->computeQueue);
	vkGetDeviceQueue(inst->device, inst->presentFamilyIdx, 0, &inst->presentQueue);

	return VKH_TRUE;
}

static void _vkh_destroy_vk_device(VKHinstance* inst)
{
	MSG_LOG("destroying Vulkan device...");

	vkDestroyDevice(inst->device, NULL);
}

VkImageView vkh_create_image_view(VKHinstance* inst, VkImage image, VkFormat format, VkImageAspectFlags aspects, uint32_t mipLevels)
{
	VkImageViewCreateInfo viewInfo = {};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspects;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = mipLevels;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView view;
	if(vkCreateImageView(inst->device, &viewInfo, NULL, &view) != VK_SUCCESS)
		ERROR_LOG("failed to create image view");

	return view;
}

void vkh_destroy_image_view(VKHinstance* inst, VkImageView view)
{
	vkDestroyImageView(inst->device, view, NULL);
}

vkh_bool_t _vkh_create_swapchain(VKHinstance* inst, uint32_t w, uint32_t h)
{
	MSG_LOG("creating Vulkan swapchain...");

	//get format and present mode:
	//---------------
	uint32_t formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(inst->physicalDevice, inst->surface, &formatCount, NULL);
	VkSurfaceFormatKHR* supportedFormats = (VkSurfaceFormatKHR*)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
	vkGetPhysicalDeviceSurfaceFormatsKHR(inst->physicalDevice, inst->surface, &formatCount, supportedFormats);

	VkSurfaceFormatKHR format = supportedFormats[0];
	for(uint32_t i = 0; i < formatCount; i++)
		if(supportedFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && supportedFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			format = supportedFormats[i];
			break;
		}

	free(supportedFormats);

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(inst->physicalDevice, inst->surface, &presentModeCount, NULL);
	VkPresentModeKHR* supportedPresentModes = (VkPresentModeKHR*)malloc(presentModeCount * sizeof(VkPresentModeKHR));
	vkGetPhysicalDeviceSurfacePresentModesKHR(inst->physicalDevice, inst->surface, &presentModeCount, supportedPresentModes);
	
	VkPresentModeKHR presentMode = supportedPresentModes[0];
	for(uint32_t i = 0; i < presentModeCount; i++)
		if(supportedPresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
			presentMode = supportedPresentModes[i];

	free(supportedPresentModes);

	//get extent:
	//---------------
	VkSurfaceCapabilitiesKHR capabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(inst->physicalDevice, inst->surface, &capabilities);

	VkExtent2D extent;
	if(capabilities.currentExtent.width != UINT32_MAX)
	{
		//TODO: does this work when the window is resized?
		extent = capabilities.currentExtent; //window already defined size for us
	}
	else
	{
		extent = (VkExtent2D){w, h};
		
		//clamping:
		extent.width = extent.width > capabilities.maxImageExtent.width ? capabilities.maxImageExtent.width : extent.width;
		extent.width = extent.width < capabilities.minImageExtent.width ? capabilities.minImageExtent.width : extent.width;

		extent.height = extent.height > capabilities.maxImageExtent.height ? capabilities.maxImageExtent.height : extent.height;
		extent.height = extent.height < capabilities.minImageExtent.height ? capabilities.minImageExtent.height : extent.height;
	}

	//get image count:
	//---------------
	uint32_t imageCount = capabilities.minImageCount + 1;
	if(capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount)
		imageCount = capabilities.maxImageCount;
	
	//get swapchain:
	//---------------
	VkSwapchainCreateInfoKHR swapchainInfo = {};
	swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainInfo.surface = inst->surface;
	swapchainInfo.minImageCount = imageCount;
	swapchainInfo.imageFormat = format.format;
	swapchainInfo.imageColorSpace = format.colorSpace;
	swapchainInfo.imageExtent = extent;
	swapchainInfo.imageArrayLayers = 1;
	swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainInfo.preTransform = capabilities.currentTransform;
	swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainInfo.presentMode = presentMode;
	swapchainInfo.clipped = VK_TRUE;
	swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

	uint32_t indices[] = {inst->graphicsComputeFamilyIdx, inst->presentFamilyIdx};

	if(inst->graphicsComputeFamilyIdx != inst->presentFamilyIdx)
	{	
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapchainInfo.queueFamilyIndexCount = 2;
		swapchainInfo.pQueueFamilyIndices = indices;
	}
	else
		swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if(vkCreateSwapchainKHR(inst->device, &swapchainInfo, NULL, &inst->swapchain) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create Vulkan swapchain");
		return VKH_FALSE;
	}

	inst->swapchainExtent = extent;
	inst->swapchainFormat = format.format;

	vkGetSwapchainImagesKHR(inst->device, inst->swapchain, &inst->swapchainImageCount, NULL);
	inst->swapchainImages     =     (VkImage*)malloc(inst->swapchainImageCount * sizeof(VkImage));
	inst->swapchainImageViews = (VkImageView*)malloc(inst->swapchainImageCount * sizeof(VkImageView));
	vkGetSwapchainImagesKHR(inst->device, inst->swapchain, &inst->swapchainImageCount, inst->swapchainImages);

	for(uint32_t i = 0; i < inst->swapchainImageCount; i++)
		inst->swapchainImageViews[i] = vkh_create_image_view(inst, inst->swapchainImages[i], inst->swapchainFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);

	return VKH_TRUE;
}

void _vkh_destroy_swapchain(VKHinstance* inst)
{
	MSG_LOG("destroying Vulkan swapchain...");

	for(uint32_t i = 0; i < inst->swapchainImageCount; i++)
		vkh_destroy_image_view(inst, inst->swapchainImageViews[i]);
	
	free(inst->swapchainImages);
	free(inst->swapchainImageViews);

	vkDestroySwapchainKHR(inst->device, inst->swapchain, NULL);
}

//----------------------------------------------------------------------------//

vkh_bool_t _vkh_create_command_pool(VKHinstance* inst)
{
	MSG_LOG("creating command pool...");

	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = inst->graphicsComputeFamilyIdx;

	if(vkCreateCommandPool(inst->device, &poolInfo, NULL, &inst->commandPool) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create command pool");
		return VKH_FALSE;
	}

	return VKH_TRUE;
}

static void _vkh_destroy_command_pool(VKHinstance* inst)
{
	MSG_LOG("destroying command pool...");

	vkDestroyCommandPool(inst->device, inst->commandPool, NULL);
}

//----------------------------------------------------------------------------//


vkh_bool_t vkh_init(VKHinstance **instance)
{
    *instance = (VKHinstance*)malloc(sizeof(VKHinstance));
	VKHinstance* inst = *instance;

    if(!_vkh_init_window(inst))
    {
		return VKH_FALSE;
    }
	if(!_vkh_create_vk_instance(inst))
	{
		return VKH_FALSE;
	}
	if(!_vkh_pick_physical_device(inst))
	{	
		return VKH_FALSE;
	}
	if(!_vkh_create_device(inst))
	{
		return VKH_FALSE;
	}
	if(!_vkh_create_swapchain(inst, Zayn->windowSize.x, Zayn->windowSize.y))
	{
		return VKH_FALSE;
	}	
	if(!_vkh_create_command_pool(inst))
	{
		return VKH_FALSE;
	}	


	return VKH_TRUE;
}

//___________ VK INIT DESTROY

void _vkh_destroy_vk_instance(VKHinstance* inst)
{
	MSG_LOG("destroying Vulkan instance...");

	vkDestroySurfaceKHR(inst->instance, inst->surface, NULL);

	#if VKH_VALIDATION_LAYERS
	{
		PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugMessenger =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(inst->instance, "vkDestroyDebugUtilsMessengerEXT");
		
		if(destroyDebugMessenger)
			destroyDebugMessenger(inst->instance, inst->debugMessenger, NULL);
		else
			ERROR_LOG("could not find function \"vkDestroyDebugUtilsMessengerEXT\"");
	}
	#endif
	
	vkDestroyInstance(inst->instance, NULL);
}

//----------------------------------------------------------------------------//
uint32_t _vkh_find_memory_type(VKHinstance* inst, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(inst->physicalDevice, &memProperties);

	for(uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
		if((typeFilter & (1 << i)) && ((memProperties.memoryTypes[i].propertyFlags & properties) == properties))
			return i;
	
	ERROR_LOG("failed to find a suitable memory type");
	return UINT32_MAX;
}
//----------------------------------------------------------------------------//
VkImage vkh_create_image(VKHinstance* inst, uint32_t w, uint32_t h, uint32_t mipLevels, VkSampleCountFlagBits samples, 
	VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory* memory)
{
	VkImageCreateInfo imageInfo = {};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = w;
	imageInfo.extent.height = h;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; //TODO: allow this to be specified, not sure if we'd ever want a concurrently shared image
	imageInfo.samples = samples;

	VkImage image;
	if(vkCreateImage(inst->device, &imageInfo, NULL, &image) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create image");
		return image;
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(inst->device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _vkh_find_memory_type(inst, memRequirements.memoryTypeBits, properties);

	if(vkAllocateMemory(inst->device, &allocInfo, NULL, memory) != VK_SUCCESS)
	{
		ERROR_LOG("failed to allocate device memory for image");
		return image;
	}

	vkBindImageMemory(inst->device, image, *memory, 0);
	return image;
}
void vkh_destroy_image(VKHinstance* inst, VkImage image, VkDeviceMemory memory)
{
	vkFreeMemory(inst->device, memory, NULL);
	vkDestroyImage(inst->device, image, NULL);
}
bool _draw_create_depth_buffer(DrawState* s)
{
	const uint32 possibleDepthFormatCount = 3;
	const VkFormat possibleDepthFormats[3] = {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT};

	bool depthFormatFound = false;
	for(int32 i = 0; i < possibleDepthFormatCount; i++)
	{
		VkFormatProperties properties;
		vkGetPhysicalDeviceFormatProperties(s->instance->physicalDevice, possibleDepthFormats[i], &properties);

		if(properties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
		{
			s->depthFormat = possibleDepthFormats[i];
			depthFormatFound = true;
			break;
		}
	}

	if(!depthFormatFound)
	{
		ERROR_LOG("failed to find a supported depth buffer format");
		return false;
	}

	s->finalDepthImage = vkh_create_image(s->instance, s->instance->swapchainExtent.width, s->instance->swapchainExtent.height, 1,
											 VK_SAMPLE_COUNT_1_BIT, s->depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
											 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->finalDepthMemory);
	s->finalDepthView = vkh_create_image_view(s->instance, s->finalDepthImage, s->depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

	return true;
}
static void _draw_destroy_depth_buffer(DrawState* s)
{
	vkh_destroy_image_view(s->instance, s->finalDepthView);
	vkh_destroy_image(s->instance, s->finalDepthImage, s->finalDepthMemory);
}
bool _draw_create_final_render_pass(DrawState* s)
{
	//create attachments:
	//---------------
	VkAttachmentDescription colorAttachment = {};
	colorAttachment.format = s->instance->swapchainFormat;
	colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	VkAttachmentDescription depthAttachment = {};
	depthAttachment.format = s->depthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//create attachment references:
	//---------------
	VkAttachmentReference colorAttachmentReference = {};
	colorAttachmentReference.attachment = 0;
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthAttachmentReference = {};
	depthAttachmentReference.attachment = 1;
	depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	//create subpass description:
	//---------------
	VkSubpassDescription subpass = {};
	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorAttachmentReference;
	subpass.pDepthStencilAttachment = &depthAttachmentReference;

	//create dependency:
	//---------------
	VkSubpassDependency attachmentDependency = {};
	attachmentDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	attachmentDependency.dstSubpass = 0;
	attachmentDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	attachmentDependency.srcAccessMask = 0;
	attachmentDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
	attachmentDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

	VkSubpassDependency dependencies[1] = {attachmentDependency};

	//create render pass:
	//---------------
	const uint32 attachmentCount = 2;
	VkAttachmentDescription attachments[2] = {colorAttachment, depthAttachment};

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = attachmentCount;
	renderPassCreateInfo.pAttachments = attachments;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = dependencies;

	if(vkCreateRenderPass(s->instance->device, &renderPassCreateInfo, nullptr, &s->finalRenderPass) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create final render pass");
		return false;
	}

	return true;
}
static void _draw_destroy_final_render_pass(DrawState* s)
{
	vkDestroyRenderPass(s->instance->device, s->finalRenderPass, NULL);
}
VkCommandBuffer vkh_start_single_time_command(VKHinstance* inst)
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = inst->commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(inst->device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);
	return commandBuffer;
}
void vkh_end_single_time_command(VKHinstance* inst, VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffer;

	vkQueueSubmit(inst->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
	vkQueueWaitIdle(inst->graphicsQueue);

	vkFreeCommandBuffers(inst->device, inst->commandPool, 1, &commandBuffer);
}
//----------------------------------------------------------------------------//
bool _draw_create_framebuffers(DrawState* s)
{
	s->framebufferCount = s->instance->swapchainImageCount;
	s->framebuffers = (VkFramebuffer*)malloc(s->framebufferCount * sizeof(VkFramebuffer));

	for(uint32 i = 0; i < s->framebufferCount; i++)
	{
		VkImageView attachments[2] = {s->instance->swapchainImageViews[i], s->finalDepthView};

		VkFramebufferCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		createInfo.renderPass = s->finalRenderPass;
		createInfo.attachmentCount = 2;
		createInfo.pAttachments = attachments;
		createInfo.width = s->instance->swapchainExtent.width;
		createInfo.height = s->instance->swapchainExtent.height;
		createInfo.layers = 1;

		if(vkCreateFramebuffer(s->instance->device, &createInfo, nullptr, &s->framebuffers[i]) != VK_SUCCESS)
		{
			ERROR_LOG("failed to create framebuffer");
			return false;
		}
	}
	return true;
}
void _draw_destroy_framebuffers(DrawState* s)
{
	for(uint32 i = 0; i < s->framebufferCount; i++)
		vkDestroyFramebuffer(s->instance->device, s->framebuffers[i], NULL);

	free(s->framebuffers);
}
bool _draw_create_command_buffers(DrawState* s)
{
	VkCommandPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = s->instance->graphicsComputeFamilyIdx;

	if(vkCreateCommandPool(s->instance->device, &poolInfo, nullptr, &s->commandPool) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create command pool");
		return false;
	}

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = s->commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = FRAMES_IN_FLIGHT;

	if(vkAllocateCommandBuffers(s->instance->device, &allocInfo, s->commandBuffers) != VK_SUCCESS)
	{
		ERROR_LOG("failed to allocate command buffers");
		return false;
	}

	return true;
}
void _draw_destroy_command_buffers(DrawState* s)
{
	vkFreeCommandBuffers(s->instance->device, s->commandPool, FRAMES_IN_FLIGHT, s->commandBuffers);
	vkDestroyCommandPool(s->instance->device, s->commandPool, NULL);
}
bool _draw_create_sync_objects(DrawState* s)
{
	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
		if(vkCreateSemaphore(s->instance->device, &semaphoreInfo, NULL, &s->imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(s->instance->device, &semaphoreInfo, NULL, &s->renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(s->instance->device, &fenceInfo, NULL, &s->inFlightFences[i]) != VK_SUCCESS)
		{
			ERROR_LOG("failed to create sync objects");
			return false;
		}
	return true;
}
void _draw_destroy_sync_objects(DrawState* s)
{
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		vkDestroySemaphore(s->instance->device, s->imageAvailableSemaphores[i], NULL);
		vkDestroySemaphore(s->instance->device, s->renderFinishedSemaphores[i], NULL);
		vkDestroyFence(s->instance->device, s->inFlightFences[i], NULL);
	}
}
VkBuffer vkh_create_buffer(VKHinstance* inst, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory* memory)
{
	VkBufferCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	createInfo.size = size;
	createInfo.usage = usage;
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkBuffer buffer;
	if(vkCreateBuffer(inst->device, &createInfo, NULL, &buffer) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create buffer");
		return buffer;
	}

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements(inst->device, buffer, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = _vkh_find_memory_type(inst, memRequirements.memoryTypeBits, properties);

	if(vkAllocateMemory(inst->device, &allocInfo, NULL, memory) != VK_SUCCESS)
	{
		ERROR_LOG("failed to allocate memory for buffer");
		return buffer;
	}
	
	vkBindBufferMemory(inst->device, buffer, *memory, 0);

	return buffer;
}
VkImage vkh_create_image(VKHinstance* inst, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory* imageMemory)
{
	VkImageCreateInfo imageInfo = {};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
    // imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkImage image;

    if (vkCreateImage(inst->device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        ERROR_LOG("failed to create image");
		return image;
    }

	VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(inst->device, image, &memRequirements);

	VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = _vkh_find_memory_type(inst, memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(inst->device, &allocInfo, nullptr, imageMemory) != VK_SUCCESS) {
        ERROR_LOG("failed to allocate memory for image");
		return image;
    }

    vkBindImageMemory(inst->device, image, *imageMemory, 0);

	return image;
}
void vkh_transition_image_layout(VKHinstance* inst, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
	VkCommandBuffer commandBuffer = vkh_start_single_time_command(inst);

	VkImageMemoryBarrier barrier = {};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout;
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = image;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		commandBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier);


	vkh_end_single_time_command(inst, commandBuffer);


}
void vkh_copy_buffer_to_image(VKHinstance* inst, VkBuffer buffer, VkImage* image, uint32_t width, uint32_t height)
{

	VkCommandBuffer commandBuffer = vkh_start_single_time_command(inst);

        VkBufferImageCopy region = {};
        region.bufferOffset = 0;
        region.bufferRowLength = 0;
        region.bufferImageHeight = 0;
        region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        region.imageSubresource.mipLevel = 1;
        region.imageSubresource.baseArrayLayer = 0;
        region.imageSubresource.layerCount = 1;
        region.imageOffset = {0, 0, 0};
        region.imageExtent = {
            width,
            height,
            1
        };

        vkCmdCopyBufferToImage(commandBuffer, buffer, *image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

        vkh_end_single_time_command(inst, commandBuffer);

}
void vkh_destroy_buffer(VKHinstance* inst, VkBuffer buffer, VkDeviceMemory memory)
{
	vkFreeMemory(inst->device, memory, NULL);
	vkDestroyBuffer(inst->device, buffer, NULL);
}
void vkh_copy_buffer(VKHinstance* inst, VkBuffer src, VkBuffer dst, VkDeviceSize size, uint64_t srcOffset, uint64_t dstOffset)
{
	VkCommandBuffer commandBuffer = vkh_start_single_time_command(inst);

	VkBufferCopy copyRegion = {};
	copyRegion.srcOffset = srcOffset;
	copyRegion.dstOffset = dstOffset;
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, src, dst, 1, &copyRegion);

	vkh_end_single_time_command(inst, commandBuffer);
}
void vkh_copy_with_staging_buf(VKHinstance* inst, VkBuffer stagingBuf, VkDeviceMemory stagingBufMem, VkBuffer buf, uint64_t size, uint64_t offset, void* data)
{
	void* mem;
	vkMapMemory(inst->device, stagingBufMem, 0, size, 0, &mem);
	memcpy(mem, data, size);
	vkUnmapMemory(inst->device, stagingBufMem);

	vkh_copy_buffer(inst, stagingBuf, buf, size, 0, offset);
}
void vkh_copy_with_staging_buf_implicit(VKHinstance* inst, VkBuffer buf, uint64_t size, uint64_t offset, void* data)
{
	VkDeviceMemory stagingBufferMemory;
	VkBuffer stagingBuffer = vkh_create_buffer(inst, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, 
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBufferMemory);

	vkh_copy_with_staging_buf(inst, stagingBuffer, stagingBufferMemory, buf, size, offset, data);

	vkh_destroy_buffer(inst, stagingBuffer, stagingBufferMemory);
}
bool vkh_copy_image_with_staging_buf(VKHinstance* inst, VkBuffer stagingBuf, VkDeviceMemory stagingBufMem, uint64_t imageSize, stbi_uc* pixels)
{
	void* data;
	vkMapMemory(inst->device, stagingBufMem, 0, imageSize, 0, &data);
    memcpy(data, pixels, static_cast<size_t>(imageSize));
	vkUnmapMemory(inst->device, stagingBufMem);

	stbi_image_free(pixels);
	// createImage(texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);
    //     transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    //         copyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight));
     //   transitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);


	return true;
}
// 			  ____    _    __  __ _____    ____ ___  ____  _____ 
// 			 / ___|  / \  |  \/  | ____|  / ___/ _ \|  _ \| ____|
// 			| |  _  / _ \ | |\/| |  _|   | |  | | | | | | |  _|  
// 			| |_| |/ ___ \| |  | | |___  | |__| |_| | |_| | |___ 
// 			 \____/_/   \_\_|  |_|_____|  \____\___/|____/|_____|
//			
//	_/----------GAME CODE-----------\_
bool _draw_create_camera_buffer(DrawState* s)
{
	VkDeviceSize bufferSize = sizeof(CameraGPU);

	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		s->cameraBuffers[i] = vkh_create_buffer(s->instance, bufferSize,
												   VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
												   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->cameraBuffersMemory[i]);
	}

	s->cameraStagingBuffer = vkh_create_buffer(s->instance, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
												  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &s->cameraStagingBufferMemory);

	return true;
}
void _draw_destroy_camera_buffer(DrawState* s)
{
	vkh_destroy_buffer(s->instance, s->cameraStagingBuffer, s->cameraStagingBufferMemory);

	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
		vkh_destroy_buffer(s->instance, s->cameraBuffers[i], s->cameraBuffersMemory[i]);
}
bool _draw_create_quad_vertex_buffer(DrawState* s)
{
	Vertex_NEW verts[4] = {{{-0.5f, 0.0f, -0.5f}, {0.0f, 0.0f}}, {{0.5f, 0.0f, -0.5f}, {1.0f, 0.0f}}, {{-0.5f, 0.0f, 0.5f}, {0.0f, 1.0f}}, {{0.5f, 0.0f, 0.5f}, {1.0f, 1.0f}}};
	uint32 indices[6] = {0, 1, 2, 1, 2, 3};

	s->quadVertexBuffer = vkh_create_buffer(s->instance, sizeof(verts),
												  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
												  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->quadVertexBufferMemory);
	vkh_copy_with_staging_buf_implicit(s->instance, s->quadVertexBuffer, sizeof(verts), 0, verts);

	s->quadIndexBuffer = vkh_create_buffer(s->instance, sizeof(indices),
												 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
												 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->quadIndexBufferMemory);
	vkh_copy_with_staging_buf_implicit(s->instance, s->quadIndexBuffer, sizeof(indices), 0, indices);

	return true;
}
void _draw_destroy_quad_vertex_buffer(DrawState* s)
{
	vkh_destroy_buffer(s->instance, s->quadVertexBuffer, s->quadVertexBufferMemory);
	vkh_destroy_buffer(s->instance, s->quadIndexBuffer, s->quadIndexBufferMemory);
}

VkImage CreateTextureImage(DrawState* s, VkImage* image1, VkDeviceMemory* imageMemory, const std::string texture_path)
{
	int texWidth, texHeight, texChannels;
    stbi_uc* pixels = stbi_load(texture_path.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    VkDeviceSize imageSize = texWidth * texHeight * 4;
	// std::cout << "tex Width: " << texWidth << " |  texHeight: " << texHeight << std::endl;

    if (!pixels) {
        throw std::runtime_error("failed to load texture image!");
    }

	VkDeviceMemory stagingBufferMemory;
	VkBuffer stagingBuffer = vkh_create_buffer(s->instance, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &stagingBufferMemory);
	
	vkh_copy_image_with_staging_buf(s->instance, stagingBuffer, stagingBufferMemory, imageSize, pixels);

	VkImage image = vkh_create_image(s->instance, texWidth, texHeight, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageMemory);	
	
	vkh_transition_image_layout(s->instance, image, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
	vkh_copy_buffer_to_image(s->instance, stagingBuffer, &image, texWidth, texHeight);
	vkh_destroy_buffer(s->instance, stagingBuffer, stagingBufferMemory);
	
	return image;
}
VkImageView CreateTextureImageView(VKHinstance* inst, VkImage image, VkFormat format)
{
        VkImageViewCreateInfo viewInfo = {};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = 1;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = 1;

        VkImageView imageView;
        if (vkCreateImageView(inst->device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;

}
bool CreateTextureSampler(DrawState* s, VkSampler* textureSampler)
{
	VkPhysicalDeviceProperties properties = {};
        vkGetPhysicalDeviceProperties(s->instance->physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo = {};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

        vkGetPhysicalDeviceProperties(s->instance->physicalDevice, &properties);
        if (vkCreateSampler(s->instance->device, &samplerInfo, nullptr, textureSampler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture sampler!");
        }
	return true;
}
bool _draw_create_quadTexture1_vertex_buffer(DrawState* s, const std::string texture_path)
{
	s->quadTexture1Image = CreateTextureImage(s, &s->quadTexture1Image, &s->quadTexture1ImageMemory, texture_path);
    s->quadTexture1TextureImageView = CreateTextureImageView(s->instance, s->quadTexture1Image, VK_FORMAT_R8G8B8A8_SRGB);
    CreateTextureSampler(s, &s->quadTexture1TextureSampler);
	
	Vertex_NEW verts[4] = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f, 0.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f},  {1.0f, 1.0f}}
};
uint32 indices[6] = {0, 1, 2, 1, 2, 3};

	s->quadTexture1VertexBuffer = vkh_create_buffer(s->instance, sizeof(verts),
												  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
												  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->quadTexture1VertexBufferMemory);
	vkh_copy_with_staging_buf_implicit(s->instance, s->quadTexture1VertexBuffer, sizeof(verts), 0, verts);

	s->quadTexture1IndexBuffer = vkh_create_buffer(s->instance, sizeof(indices),
												 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
												 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &s->quadTexture1IndexBufferMemory);
	vkh_copy_with_staging_buf_implicit(s->instance, s->quadTexture1IndexBuffer, sizeof(indices), 0, indices);

	return true;
}
bool _draw_create_model1_load_model(DrawState* s, const std::string texture_path, const std::string model_path)
{
	s->model1Image = CreateTextureImage(s, &s->model1Image, &s->model1ImageMemory, texture_path);
    //s->model1TextureImageView = CreateTextureImageView(s->instance, s->model1Image, VK_FORMAT_R8G8B8A8_SRGB);
    CreateTextureSampler(s, &s->model1TextureSampler);

	tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str()))
	{
		throw std::runtime_error(warn + err);
	}

	// std::unordered_map<Vertex_NEW, uint32_t> uniqueVertices{};

	for (const auto &shape : shapes)
	{
		for (const auto &index : shape.mesh.indices)
		{
			Vertex_NEW vertex{};

			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2]};

			vertex.texCoord = {
				attrib.texcoords[2 * index.texcoord_index + 0],
				1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
			// vertex.color = {1.0f, 1.0f, 1.0f};
			// if (uniqueVertices.count(vertex) == 0)
			// {
			// 	uniqueVertices[vertex] = static_cast<uint32_t>(s->model1vertices.size());
			// 	s->model1vertices.push_back(vertex);
			// }
			// s->model1indices.push_back(uniqueVertices[vertex]);
		}
	}
	return true;	
}
bool _draw_create_model1_vertex_buffer(DrawState* s)
{
	return true;
}
static void _draw_destroy_model1_vertex_buffer(DrawState* s)
{
	vkh_destroy_buffer(s->instance, s->model1VertexBuffer, s->model1VertexBufferMemory);
	vkh_destroy_buffer(s->instance, s->model1IndexBuffer, s->model1IndexBufferMemory);
}
//----------------------------------------------------------------------------//
VKHgraphicsPipeline* vkh_pipeline_create()
{
	VKHgraphicsPipeline* pipeline = (VKHgraphicsPipeline*)malloc(sizeof(VKHgraphicsPipeline));
	if(!pipeline)
		return NULL;

	pipeline->descSetBindings       = MakeDynamicArray<VkDescriptorSetLayoutBinding>(&Zayn->permanentMemArena, 100);
	pipeline->dynamicStates         = MakeDynamicArray<VkDynamicState>(&Zayn->permanentMemArena, 100);
	pipeline->vertInputBindings     = MakeDynamicArray<VkVertexInputBindingDescription>(&Zayn->permanentMemArena, 100);
	pipeline->vertInputAttribs      = MakeDynamicArray<VkVertexInputAttributeDescription>(&Zayn->permanentMemArena, 100);
	pipeline->viewports             = MakeDynamicArray<VkViewport>(&Zayn->permanentMemArena, 100);
	pipeline->scissors              = MakeDynamicArray<VkRect2D>(&Zayn->permanentMemArena, 100);
	pipeline->colorBlendAttachments = MakeDynamicArray<VkPipelineColorBlendAttachmentState>(&Zayn->permanentMemArena, 100);
	pipeline->pushConstants         = MakeDynamicArray<VkPushConstantRange>(&Zayn->permanentMemArena, 100);

	pipeline->vertShader = VK_NULL_HANDLE;
	pipeline->fragShader = VK_NULL_HANDLE;

	pipeline->inputAssemblyState = (VkPipelineInputAssemblyStateCreateInfo){};
	pipeline->inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	pipeline->inputAssemblyState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	pipeline->inputAssemblyState.primitiveRestartEnable = VK_FALSE;

	pipeline->tesselationState = (VkPipelineTessellationStateCreateInfo){};
	pipeline->tesselationState.sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO;
	pipeline->tesselationState.patchControlPoints = 1;

	pipeline->rasterState = (VkPipelineRasterizationStateCreateInfo){};
	pipeline->rasterState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	pipeline->rasterState.depthClampEnable = VK_FALSE;
	pipeline->rasterState.rasterizerDiscardEnable = VK_FALSE;
	pipeline->rasterState.polygonMode = VK_POLYGON_MODE_FILL;
	pipeline->rasterState.cullMode = VK_CULL_MODE_NONE;
	pipeline->rasterState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
	pipeline->rasterState.depthBiasEnable = VK_FALSE;
	pipeline->rasterState.depthBiasConstantFactor = 0.0f;
	pipeline->rasterState.depthBiasClamp = 0.0f;
	pipeline->rasterState.depthBiasSlopeFactor = 0.0f;
	pipeline->rasterState.lineWidth = 1.0f; //we dont allow this to be changed as its not really useful or supported

	pipeline->multisampleState = (VkPipelineMultisampleStateCreateInfo){};
	pipeline->multisampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	pipeline->multisampleState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	pipeline->multisampleState.sampleShadingEnable = VK_FALSE;
	pipeline->multisampleState.minSampleShading = 1.0f;
	pipeline->multisampleState.pSampleMask = NULL;
	pipeline->multisampleState.alphaToCoverageEnable = VK_FALSE;
	pipeline->multisampleState.alphaToOneEnable = VK_FALSE;

	pipeline->depthStencilState = (VkPipelineDepthStencilStateCreateInfo){};
	pipeline->depthStencilState.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	pipeline->depthStencilState.depthTestEnable = VK_TRUE;
	pipeline->depthStencilState.depthWriteEnable = VK_TRUE;
	pipeline->depthStencilState.depthCompareOp = VK_COMPARE_OP_LESS;
	pipeline->depthStencilState.depthBoundsTestEnable = VK_FALSE;
	pipeline->depthStencilState.stencilTestEnable = VK_FALSE;
	pipeline->depthStencilState.front = (VkStencilOpState){};
	pipeline->depthStencilState.back = (VkStencilOpState){};
	pipeline->depthStencilState.minDepthBounds = 0.0f;
	pipeline->depthStencilState.maxDepthBounds = 1.0f;

	pipeline->colorBlendState = (VkPipelineColorBlendStateCreateInfo){};
	pipeline->colorBlendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	pipeline->colorBlendState.logicOpEnable = VK_FALSE;
	pipeline->colorBlendState.logicOp = VK_LOGIC_OP_COPY;
	pipeline->colorBlendState.blendConstants[0] = 0.0f;
	pipeline->colorBlendState.blendConstants[1] = 0.0f;
	pipeline->colorBlendState.blendConstants[2] = 0.0f;
	pipeline->colorBlendState.blendConstants[3] = 0.0f;

	pipeline->generated = VKH_FALSE;
	pipeline->descriptorLayout = VK_NULL_HANDLE;
	pipeline->layout           = VK_NULL_HANDLE;
	pipeline->pipeline         = VK_NULL_HANDLE;

	return pipeline;
}
void vkh_pipeline_destroy(VKHgraphicsPipeline* pipeline)
{
	if(pipeline->generated)
	{
		ERROR_LOG("you must call vkh_pipeline_cleanup() before calling vkh_pipeline_destroy()");
		return;
	}

	DynamicArrayClear(&pipeline->descSetBindings);
	DynamicArrayClear(&pipeline->dynamicStates);
	DynamicArrayClear(&pipeline->vertInputBindings);
	DynamicArrayClear(&pipeline->vertInputAttribs);
	DynamicArrayClear(&pipeline->viewports);
	DynamicArrayClear(&pipeline->scissors);
	DynamicArrayClear(&pipeline->colorBlendAttachments);
	DynamicArrayClear(&pipeline->pushConstants);

	free(pipeline);
}
uint32_t* vkh_load_spirv(const char* path, uint64_t* size)
{
#if _MSC_VER
	FILE* fptr;
	fopen_s(&fptr, path, "rb");
#else
	FILE* fptr = fopen(path, "rb");
#endif
	if(!fptr)
	{
		ERROR_LOG("failed to open spirv file");
		return NULL;
	}

	fseek(fptr, 0, SEEK_END);
	*size = ftell(fptr);

	fseek(fptr, 0, SEEK_SET);
	uint32_t* code = (uint32_t*)malloc(*size);
	fread(code, *size, 1, fptr);

	fclose(fptr);
	return code;
}
void vkh_free_spirv(uint32_t* code)
{
	free(code);
}
VkShaderModule vkh_create_shader_module(VKHinstance* inst, uint64_t codeSize, uint32_t* code)
{
	VkShaderModuleCreateInfo moduleInfo = {};
	moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	moduleInfo.codeSize = codeSize;
	moduleInfo.pCode = code;

	VkShaderModule module;
	if(vkCreateShaderModule(inst->device, &moduleInfo, NULL, &module) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create shader module");
		return module;
	}

	return module;
}
void vkh_destroy_shader_module(VKHinstance* inst, VkShaderModule module)
{
	vkDestroyShaderModule(inst->device, module, NULL);
}
void vkh_pipeline_set_vert_shader(VKHgraphicsPipeline* pipeline, VkShaderModule shader)
{
	pipeline->vertShader = shader;
}
void vkh_pipeline_set_frag_shader(VKHgraphicsPipeline* pipeline, VkShaderModule shader)
{
	pipeline->fragShader = shader;
}
//----------------------------------------------------------------------------//
void vkh_pipeline_add_desc_set_binding(VKHgraphicsPipeline* pipeline, VkDescriptorSetLayoutBinding binding)
{
	PushBack(&pipeline->descSetBindings, binding);
}
void vkh_pipeline_add_dynamic_state(VKHgraphicsPipeline* pipeline, VkDynamicState state)
{
	PushBack(&pipeline->dynamicStates, state);
}
void vkh_pipeline_add_vertex_input_binding(VKHgraphicsPipeline* pipeline, VkVertexInputBindingDescription binding)
{
	PushBack(&pipeline->vertInputBindings, binding);
}
void vkh_pipeline_add_vertex_input_attrib(VKHgraphicsPipeline* pipeline, VkVertexInputAttributeDescription attrib)
{
	PushBack(&pipeline->vertInputAttribs, attrib);
}
void vkh_pipeline_add_viewport(VKHgraphicsPipeline* pipeline, VkViewport viewport)
{
	PushBack(&pipeline->viewports, viewport);
}
void vkh_pipeline_add_color_blend_attachment(VKHgraphicsPipeline* pipeline, VkPipelineColorBlendAttachmentState attachment)
{
	PushBack(&pipeline->colorBlendAttachments, attachment);
}
void vkh_pipeline_add_push_constant(VKHgraphicsPipeline* pipeline, VkPushConstantRange pushConstant)
{
	PushBack(&pipeline->pushConstants, pushConstant);
}
void vkh_pipeline_set_input_assembly_state(VKHgraphicsPipeline* pipeline, VkPrimitiveTopology topology, VkBool32 primitiveRestart)
{
	pipeline->inputAssemblyState.topology = topology;
	pipeline->inputAssemblyState.primitiveRestartEnable = primitiveRestart;
}
void vkh_pipeline_set_raster_state(VKHgraphicsPipeline* pipeline, VkBool32 depthClamp, VkBool32 rasterDiscard, VkPolygonMode polyMode,
                                   VkCullModeFlags cullMode, VkFrontFace frontFace, VkBool32 depthBias, float biasConstFactor, float biasClamp, float biasSlopeFactor)
{
	pipeline->rasterState.depthClampEnable = depthClamp;
	pipeline->rasterState.rasterizerDiscardEnable = rasterDiscard;
	pipeline->rasterState.polygonMode = polyMode;
	pipeline->rasterState.cullMode = cullMode;
	pipeline->rasterState.frontFace = frontFace;
	pipeline->rasterState.depthBiasEnable = depthBias;
	pipeline->rasterState.depthBiasConstantFactor = biasConstFactor;
	pipeline->rasterState.depthBiasClamp = biasClamp;
	pipeline->rasterState.depthBiasSlopeFactor = biasSlopeFactor;
}
void vkh_pipeline_set_multisample_state(VKHgraphicsPipeline* pipeline, VkSampleCountFlagBits rasterSamples, VkBool32 sampleShading,
                                        float minSampleShading, const VkSampleMask* sampleMask, VkBool32 alphaToCoverage, VkBool32 alphaToOne)
{
	pipeline->multisampleState.rasterizationSamples = rasterSamples;
	pipeline->multisampleState.sampleShadingEnable = sampleShading;
	pipeline->multisampleState.minSampleShading = minSampleShading;
	pipeline->multisampleState.pSampleMask = sampleMask;
	pipeline->multisampleState.alphaToCoverageEnable = alphaToCoverage;
	pipeline->multisampleState.alphaToOneEnable = alphaToOne;
}
void vkh_pipeline_set_depth_stencil_state(VKHgraphicsPipeline* pipeline, VkBool32 depthTest, VkBool32 depthWrite, VkCompareOp depthCompareOp,
                                          VkBool32 depthBoundsTest, VkBool32 stencilTest, VkStencilOpState front, VkStencilOpState back, float minDepthBound, float maxDepthBound)
{
	pipeline->depthStencilState.depthTestEnable = depthTest;
	pipeline->depthStencilState.depthWriteEnable = depthWrite;
	pipeline->depthStencilState.depthCompareOp = depthCompareOp;
	pipeline->depthStencilState.depthBoundsTestEnable = depthBoundsTest;
	pipeline->depthStencilState.stencilTestEnable = stencilTest;
	pipeline->depthStencilState.front = front;
	pipeline->depthStencilState.back = back;
	pipeline->depthStencilState.minDepthBounds = minDepthBound;
	pipeline->depthStencilState.maxDepthBounds = maxDepthBound;
}
void vkh_pipeline_set_color_blend_state(VKHgraphicsPipeline* pipeline, VkBool32 logicOpEnable, VkLogicOp logicOp, float rBlendConstant,
                                        float gBlendConstant, float bBlendConstant, float aBlendConstant)
{
	pipeline->colorBlendState.logicOpEnable = logicOpEnable;
	pipeline->colorBlendState.logicOp = logicOp;
	pipeline->colorBlendState.blendConstants[0] = rBlendConstant;
	pipeline->colorBlendState.blendConstants[1] = gBlendConstant;
	pipeline->colorBlendState.blendConstants[2] = bBlendConstant;
	pipeline->colorBlendState.blendConstants[3] = aBlendConstant;
}
//----------------------------------------------------------------------------//
vkh_bool_t vkh_pipeline_generate(VKHgraphicsPipeline* pipeline, VKHinstance* inst, VkRenderPass renderPass, uint32_t subpass)
{
	if(pipeline->generated)
	{
		ERROR_LOG("pipeline has already been generated, call vkh_pipeline_cleanup() before generating again");
		return VKH_FALSE;
	}

	//create descriptor set layout:
	//---------------
	VkDescriptorSetLayoutCreateInfo descSetLayoutInfo = {};
	descSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descSetLayoutInfo.bindingCount = (uint32_t)pipeline->descSetBindings.count;
	descSetLayoutInfo.pBindings = &pipeline->descSetBindings[0];

	if(vkCreateDescriptorSetLayout(inst->device, &descSetLayoutInfo, NULL, &pipeline->descriptorLayout) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create pipeline descriptor set layout");
		return VKH_FALSE;
	}

	//create pipeline layout:
	//---------------
	VkPipelineLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	layoutInfo.setLayoutCount = 1; //TODO: support more than 1 descriptor layout
	layoutInfo.pSetLayouts = &pipeline->descriptorLayout;
	layoutInfo.pushConstantRangeCount = (uint32_t)pipeline->pushConstants.count;
	layoutInfo.pPushConstantRanges = &pipeline->pushConstants[0];

	if(vkCreatePipelineLayout(inst->device, &layoutInfo, NULL, &pipeline->layout) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create pipeline layout");

		vkDestroyDescriptorSetLayout(inst->device, pipeline->descriptorLayout, NULL);
		return VKH_FALSE;
	}

	//create intermediate create infos:mod
	//---------------
	DynamicArray<VkPipelineShaderStageCreateInfo> shaderStages = MakeDynamicArray<VkPipelineShaderStageCreateInfo>(&Zayn->permanentMemArena, 100);
	
	if(pipeline->vertShader != VK_NULL_HANDLE)
	{
		VkPipelineShaderStageCreateInfo vertStage = {};
		vertStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertStage.module = pipeline->vertShader;
		vertStage.pName = "main";

		PushBack(&shaderStages, vertStage);
	}

	if(pipeline->fragShader != VK_NULL_HANDLE)
	{
		VkPipelineShaderStageCreateInfo fragStage = {};
		fragStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragStage.module = pipeline->fragShader;
		fragStage.pName = "main";

		PushBack(&shaderStages, fragStage);
	}

	VkPipelineVertexInputStateCreateInfo vertInputInfo = {};
	vertInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertInputInfo.vertexBindingDescriptionCount = (uint32_t)pipeline->vertInputBindings.count;
	vertInputInfo.pVertexBindingDescriptions = &pipeline->vertInputBindings[0];
	vertInputInfo.vertexAttributeDescriptionCount = (uint32_t)pipeline->vertInputAttribs.count;
	vertInputInfo.pVertexAttributeDescriptions = &pipeline->vertInputAttribs[0];

	VkPipelineViewportStateCreateInfo viewportInfo = {};
	viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportInfo.viewportCount = (uint32_t)pipeline->viewports.count;
	viewportInfo.pViewports = &pipeline->viewports[0];
	viewportInfo.scissorCount = (uint32_t)pipeline->scissors.count;
	viewportInfo.pScissors = &pipeline->scissors[0];

	for(int32_t i = 0; i < (int32_t)pipeline->dynamicStates.count; i++) //if dynamic, only 1 viewport/scissor
	{
		VkDynamicState dynState = *(VkDynamicState*)&pipeline->dynamicStates[i];

		if(dynState == VK_DYNAMIC_STATE_VIEWPORT)
			viewportInfo.viewportCount = 1;
		if(dynState == VK_DYNAMIC_STATE_SCISSOR)
			viewportInfo.scissorCount = 1;
	}

	VkPipelineDynamicStateCreateInfo dynamicStateInfo = {};
	dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateInfo.dynamicStateCount = (uint32_t)pipeline->dynamicStates.count;
	dynamicStateInfo.pDynamicStates = &pipeline->dynamicStates[0];

	pipeline->colorBlendState.attachmentCount = (uint32_t)pipeline->colorBlendAttachments.count;
	pipeline->colorBlendState.pAttachments = &pipeline->colorBlendAttachments[0];

	//create pipeline:
	//---------------
	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = (uint32_t)shaderStages.count;
	pipelineInfo.pStages = &shaderStages[0];
	pipelineInfo.pVertexInputState = &vertInputInfo;
	pipelineInfo.pInputAssemblyState = &pipeline->inputAssemblyState;
	pipelineInfo.pTessellationState = &pipeline->tesselationState;
	pipelineInfo.pViewportState = &viewportInfo;
	pipelineInfo.pRasterizationState = &pipeline->rasterState;
	pipelineInfo.pMultisampleState = &pipeline->multisampleState;
	pipelineInfo.pDepthStencilState = &pipeline->depthStencilState;
	pipelineInfo.pColorBlendState = &pipeline->colorBlendState;
	pipelineInfo.pDynamicState = &dynamicStateInfo;
	pipelineInfo.layout = pipeline->layout;
	pipelineInfo.renderPass = renderPass;
	pipelineInfo.subpass = subpass;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
	pipelineInfo.basePipelineIndex = -1;

	if(vkCreateGraphicsPipelines(inst->device, VK_NULL_HANDLE,  1, &pipelineInfo, NULL, &pipeline->pipeline) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create graphics pipeline");

		vkDestroyPipelineLayout(inst->device, pipeline->layout, NULL);
		vkDestroyDescriptorSetLayout(inst->device, pipeline->descriptorLayout, NULL);
		return VKH_FALSE;
	}

	//cleanup:
	//---------------
	DynamicArrayClear(&shaderStages);

	pipeline->generated = VKH_TRUE;
	return VKH_TRUE;
}
bool _draw_create_quad_pipeline(DrawState* s)
{
	//create pipeline object:
	//---------------
	s->gridPipeline = vkh_pipeline_create();
	if(!s->gridPipeline)
		return false;

	//set shaders:
	//---------------
	uint64 vertCodeSize, fragCodeSize;
	uint32 *vertCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/quad_vert.spv", &vertCodeSize);
	uint32 *fragCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/quad_frag.spv", &fragCodeSize);

	VkShaderModule vertModule = vkh_create_shader_module(s->instance, vertCodeSize, vertCode);
	VkShaderModule fragModule = vkh_create_shader_module(s->instance, fragCodeSize, fragCode);

	vkh_pipeline_set_vert_shader(s->gridPipeline, vertModule);
	vkh_pipeline_set_frag_shader(s->gridPipeline, fragModule);

	//add descriptor set layout bindings:
	//---------------
	VkDescriptorSetLayoutBinding storageLayoutBinding = {};
	storageLayoutBinding.binding = 0;
	storageLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	storageLayoutBinding.descriptorCount = 1; //camera
	storageLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	storageLayoutBinding.pImmutableSamplers = nullptr;

	vkh_pipeline_add_desc_set_binding(s->gridPipeline, storageLayoutBinding);

	//add dynamic states:
	//---------------
	vkh_pipeline_add_dynamic_state(s->gridPipeline, VK_DYNAMIC_STATE_VIEWPORT);
	vkh_pipeline_add_dynamic_state(s->gridPipeline, VK_DYNAMIC_STATE_SCISSOR);

	//add vertex input info:
	//---------------
	VkVertexInputBindingDescription vertBindingDescription = {};
	vertBindingDescription.binding = 0;
	vertBindingDescription.stride = sizeof(Vertex_NEW);
	vertBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vkh_pipeline_add_vertex_input_binding(s->gridPipeline, vertBindingDescription);

	VkVertexInputAttributeDescription vertPositionAttrib = {};
	vertPositionAttrib.binding = 0;
	vertPositionAttrib.location = 0;
	vertPositionAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertPositionAttrib.offset = offsetof(Vertex_NEW, pos);

	VkVertexInputAttributeDescription vertTexCoordAttrib = {};
	vertTexCoordAttrib.binding = 0;
	vertTexCoordAttrib.location = 1;
	vertTexCoordAttrib.format = VK_FORMAT_R32G32_SFLOAT;
	vertTexCoordAttrib.offset = offsetof(Vertex_NEW, texCoord);

	vkh_pipeline_add_vertex_input_attrib(s->gridPipeline, vertPositionAttrib);
	vkh_pipeline_add_vertex_input_attrib(s->gridPipeline, vertTexCoordAttrib);

	//add color blend attachments:
	//---------------
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	vkh_pipeline_add_color_blend_attachment(s->gridPipeline, colorBlendAttachment);

	//add push constsants:
	//---------------
	VkPushConstantRange vertPushConstant = {};
	vertPushConstant.offset = 0;
	vertPushConstant.size = sizeof(GridParamsVertGPU);
	vertPushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPushConstantRange fragPushConstant = {};
	fragPushConstant.offset = sizeof(GridParamsVertGPU);
	fragPushConstant.size = sizeof(GridParamsFragGPU);
	fragPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	vkh_pipeline_add_push_constant(s->gridPipeline, vertPushConstant);
	vkh_pipeline_add_push_constant(s->gridPipeline, fragPushConstant);

	//set states:
	//---------------
	vkh_pipeline_set_input_assembly_state(s->gridPipeline, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	vkh_pipeline_set_raster_state(s->gridPipeline, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
		VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f);

	vkh_pipeline_set_multisample_state(s->gridPipeline, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, NULL, VK_FALSE, VK_FALSE);

	vkh_pipeline_set_depth_stencil_state(s->gridPipeline, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

	vkh_pipeline_set_color_blend_state(s->gridPipeline, VK_FALSE, VK_LOGIC_OP_COPY, 0.0f, 0.0f, 0.0f, 0.0f);

	//generate pipeline:
	//---------------
	if(!vkh_pipeline_generate(s->gridPipeline, s->instance, s->finalRenderPass, 0))
		return false;

	//cleanup:
	//---------------
	vkh_free_spirv(vertCode);
	vkh_free_spirv(fragCode);

	vkh_destroy_shader_module(s->instance, vertModule);
	vkh_destroy_shader_module(s->instance, fragModule);

	return true;
}
bool _draw_create_model1_pipeline(DrawState* s)
{
	//create pipeline object:
	//---------------
	s->model1Pipeline = vkh_pipeline_create();
	if(!s->model1Pipeline)
		return false;
	
	//set shaders:
	//---------------
	uint64 vertCodeSize, fragCodeSize;
	uint32 *vertCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/model1_vert.spv", &vertCodeSize);
	uint32 *fragCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/model1_frag.spv", &fragCodeSize);

	VkShaderModule vertModule = vkh_create_shader_module(s->instance, vertCodeSize, vertCode);
	VkShaderModule fragModule = vkh_create_shader_module(s->instance, fragCodeSize, fragCode);

	vkh_pipeline_set_vert_shader(s->model1Pipeline, vertModule);
	vkh_pipeline_set_frag_shader(s->model1Pipeline, fragModule);

	//add descriptor set layout bindings:
	//---------------
	VkDescriptorSetLayoutBinding storageLayoutBinding = {};
	storageLayoutBinding.binding = 0;
	storageLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	storageLayoutBinding.descriptorCount = 1; //camera
	storageLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	storageLayoutBinding.pImmutableSamplers = nullptr;

	vkh_pipeline_add_desc_set_binding(s->model1Pipeline, storageLayoutBinding);

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	vkh_pipeline_add_desc_set_binding(s->model1Pipeline, samplerLayoutBinding);

	//add dynamic states:
	//---------------
	vkh_pipeline_add_dynamic_state(s->model1Pipeline, VK_DYNAMIC_STATE_VIEWPORT);
	vkh_pipeline_add_dynamic_state(s->model1Pipeline, VK_DYNAMIC_STATE_SCISSOR);

	//add vertex input info:
	//---------------
	VkVertexInputBindingDescription vertBindingDescription = {};
	vertBindingDescription.binding = 0;
	vertBindingDescription.stride = sizeof(Vertex_NEW);
	vertBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vkh_pipeline_add_vertex_input_binding(s->model1Pipeline, vertBindingDescription);

	VkVertexInputAttributeDescription vertPositionAttrib = {};
	vertPositionAttrib.binding = 0;
	vertPositionAttrib.location = 0;
	vertPositionAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertPositionAttrib.offset = offsetof(Vertex_NEW, pos);

	VkVertexInputAttributeDescription vertTexCoordAttrib = {};
	vertTexCoordAttrib.binding = 0;
	vertTexCoordAttrib.location = 1;
	vertTexCoordAttrib.format = VK_FORMAT_R32G32_SFLOAT;
	vertTexCoordAttrib.offset = offsetof(Vertex_NEW, texCoord);

	vkh_pipeline_add_vertex_input_attrib(s->model1Pipeline, vertPositionAttrib);
	vkh_pipeline_add_vertex_input_attrib(s->model1Pipeline, vertTexCoordAttrib);

	//add color blend attachments:
	//---------------
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	vkh_pipeline_add_color_blend_attachment(s->model1Pipeline, colorBlendAttachment);

	//add push constsants:
	//---------------
	VkPushConstantRange vertPushConstant = {};
	vertPushConstant.offset = 0;
	vertPushConstant.size = sizeof(GridParamsVertGPU);
	vertPushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	vkh_pipeline_add_push_constant(s->model1Pipeline, vertPushConstant);

	//set states:
	//---------------
	vkh_pipeline_set_input_assembly_state(s->model1Pipeline, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	vkh_pipeline_set_raster_state(s->model1Pipeline, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
		VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f);

	vkh_pipeline_set_multisample_state(s->model1Pipeline, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, NULL, VK_FALSE, VK_FALSE);

	vkh_pipeline_set_depth_stencil_state(s->model1Pipeline, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

	vkh_pipeline_set_color_blend_state(s->model1Pipeline, VK_FALSE, VK_LOGIC_OP_COPY, 0.0f, 0.0f, 0.0f, 0.0f);

	//generate pipeline:
	//---------------
	if(!vkh_pipeline_generate(s->model1Pipeline, s->instance, s->finalRenderPass, 0))
		return false;

	//cleanup:
	//---------------
	vkh_free_spirv(vertCode);
	vkh_free_spirv(fragCode);

	vkh_destroy_shader_module(s->instance, vertModule);
	vkh_destroy_shader_module(s->instance, fragModule);

	return true;
}
bool _draw_create_quadTexture1_pipeline(DrawState* s)
{	
	//create pipeline object:
	//---------------
	s->quadTexture1Pipeline = vkh_pipeline_create();
	if(!s->quadTexture1Pipeline)
		return false;
	
	//set shaders:
	//---------------
	uint64 vertCodeSize, fragCodeSize;
	uint32 *vertCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/quad_texture1_vert.spv", &vertCodeSize);
	uint32 *fragCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/quad_texture1_frag.spv", &fragCodeSize);

	VkShaderModule vertModule = vkh_create_shader_module(s->instance, vertCodeSize, vertCode);
	VkShaderModule fragModule = vkh_create_shader_module(s->instance, fragCodeSize, fragCode);

	vkh_pipeline_set_vert_shader(s->quadTexture1Pipeline, vertModule);
	vkh_pipeline_set_frag_shader(s->quadTexture1Pipeline, fragModule);

	//add descriptor set layout bindings:
	//---------------
	VkDescriptorSetLayoutBinding storageLayoutBinding = {};
	storageLayoutBinding.binding = 0;
	storageLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	storageLayoutBinding.descriptorCount = 1; //camera
	storageLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	storageLayoutBinding.pImmutableSamplers = nullptr;

	vkh_pipeline_add_desc_set_binding(s->quadTexture1Pipeline, storageLayoutBinding);

	VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = nullptr;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	vkh_pipeline_add_desc_set_binding(s->quadTexture1Pipeline, samplerLayoutBinding);

	//add dynamic states:
	//---------------
	vkh_pipeline_add_dynamic_state(s->quadTexture1Pipeline, VK_DYNAMIC_STATE_VIEWPORT);
	vkh_pipeline_add_dynamic_state(s->quadTexture1Pipeline, VK_DYNAMIC_STATE_SCISSOR);

	//add vertex input info:
	//---------------
	VkVertexInputBindingDescription vertBindingDescription = {};
	vertBindingDescription.binding = 0;
	vertBindingDescription.stride = sizeof(Vertex_NEW);
	vertBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vkh_pipeline_add_vertex_input_binding(s->quadTexture1Pipeline, vertBindingDescription);

	VkVertexInputAttributeDescription vertPositionAttrib = {};
	vertPositionAttrib.binding = 0;
	vertPositionAttrib.location = 0;
	vertPositionAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertPositionAttrib.offset = offsetof(Vertex_NEW, pos);

	VkVertexInputAttributeDescription vertTexCoordAttrib = {};
	vertTexCoordAttrib.binding = 0;
	vertTexCoordAttrib.location = 1;
	vertTexCoordAttrib.format = VK_FORMAT_R32G32_SFLOAT;
	vertTexCoordAttrib.offset = offsetof(Vertex_NEW, texCoord);

	vkh_pipeline_add_vertex_input_attrib(s->quadTexture1Pipeline, vertPositionAttrib);
	vkh_pipeline_add_vertex_input_attrib(s->quadTexture1Pipeline, vertTexCoordAttrib);

	//add color blend attachments:
	//---------------
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	vkh_pipeline_add_color_blend_attachment(s->quadTexture1Pipeline, colorBlendAttachment);

	//add push constsants:
	//---------------
	VkPushConstantRange vertPushConstant = {};
	vertPushConstant.offset = 0;
	vertPushConstant.size = sizeof(GridParamsVertGPU);
	vertPushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	vkh_pipeline_add_push_constant(s->quadTexture1Pipeline, vertPushConstant);

	//set states:
	//---------------
	vkh_pipeline_set_input_assembly_state(s->quadTexture1Pipeline, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	vkh_pipeline_set_raster_state(s->quadTexture1Pipeline, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
		VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f);

	vkh_pipeline_set_multisample_state(s->quadTexture1Pipeline, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, NULL, VK_FALSE, VK_FALSE);

	vkh_pipeline_set_depth_stencil_state(s->quadTexture1Pipeline, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

	vkh_pipeline_set_color_blend_state(s->quadTexture1Pipeline, VK_FALSE, VK_LOGIC_OP_COPY, 0.0f, 0.0f, 0.0f, 0.0f);

	//generate pipeline:
	//---------------
	if(!vkh_pipeline_generate(s->quadTexture1Pipeline, s->instance, s->finalRenderPass, 0))
		return false;

	//cleanup:
	//---------------
	vkh_free_spirv(vertCode);
	vkh_free_spirv(fragCode);

	vkh_destroy_shader_module(s->instance, vertModule);
	vkh_destroy_shader_module(s->instance, fragModule);

	return true;
}
bool _draw_create_grid_pipeline(DrawState* s)
{
	//create pipeline object:
	//---------------
	s->gridPipeline = vkh_pipeline_create();
	if(!s->gridPipeline)
		return false;

	//set shaders:
	//---------------
	uint64 vertCodeSize, fragCodeSize;
	uint32 *vertCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/grid_vert.spv", &vertCodeSize);
	uint32 *fragCode = vkh_load_spirv("/Users/socki/dev/zayn/src/renderer/shaders/grid_frag.spv", &fragCodeSize);

	VkShaderModule vertModule = vkh_create_shader_module(s->instance, vertCodeSize, vertCode);
	VkShaderModule fragModule = vkh_create_shader_module(s->instance, fragCodeSize, fragCode);

	vkh_pipeline_set_vert_shader(s->gridPipeline, vertModule);
	vkh_pipeline_set_frag_shader(s->gridPipeline, fragModule);

	//add descriptor set layout bindings:
	//---------------
	VkDescriptorSetLayoutBinding storageLayoutBinding = {};
	storageLayoutBinding.binding = 0;
	storageLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	storageLayoutBinding.descriptorCount = 1; //camera
	storageLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	storageLayoutBinding.pImmutableSamplers = nullptr;

	vkh_pipeline_add_desc_set_binding(s->gridPipeline, storageLayoutBinding);

	//add dynamic states:
	//---------------
	vkh_pipeline_add_dynamic_state(s->gridPipeline, VK_DYNAMIC_STATE_VIEWPORT);
	vkh_pipeline_add_dynamic_state(s->gridPipeline, VK_DYNAMIC_STATE_SCISSOR);

	//add vertex input info:
	//---------------
	VkVertexInputBindingDescription vertBindingDescription = {};
	vertBindingDescription.binding = 0;
	vertBindingDescription.stride = sizeof(Vertex_NEW);
	vertBindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

	vkh_pipeline_add_vertex_input_binding(s->gridPipeline, vertBindingDescription);

	VkVertexInputAttributeDescription vertPositionAttrib = {};
	vertPositionAttrib.binding = 0;
	vertPositionAttrib.location = 0;
	vertPositionAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
	vertPositionAttrib.offset = offsetof(Vertex_NEW, pos);

	VkVertexInputAttributeDescription vertTexCoordAttrib = {};
	vertTexCoordAttrib.binding = 0;
	vertTexCoordAttrib.location = 1;
	vertTexCoordAttrib.format = VK_FORMAT_R32G32_SFLOAT;
	vertTexCoordAttrib.offset = offsetof(Vertex_NEW, texCoord);

	vkh_pipeline_add_vertex_input_attrib(s->gridPipeline, vertPositionAttrib);
	vkh_pipeline_add_vertex_input_attrib(s->gridPipeline, vertTexCoordAttrib);

	//add color blend attachments:
	//---------------
	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

	vkh_pipeline_add_color_blend_attachment(s->gridPipeline, colorBlendAttachment);

	//add push constsants:
	//---------------
	VkPushConstantRange vertPushConstant = {};
	vertPushConstant.offset = 0;
	vertPushConstant.size = sizeof(GridParamsVertGPU);
	vertPushConstant.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

	VkPushConstantRange fragPushConstant = {};
	fragPushConstant.offset = sizeof(GridParamsVertGPU);
	fragPushConstant.size = sizeof(GridParamsFragGPU);
	fragPushConstant.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	vkh_pipeline_add_push_constant(s->gridPipeline, vertPushConstant);
	vkh_pipeline_add_push_constant(s->gridPipeline, fragPushConstant);

	//set states:
	//---------------
	vkh_pipeline_set_input_assembly_state(s->gridPipeline, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FALSE);

	vkh_pipeline_set_raster_state(s->gridPipeline, VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE,
		VK_FRONT_FACE_COUNTER_CLOCKWISE, VK_FALSE, 0.0f, 0.0f, 0.0f);

	vkh_pipeline_set_multisample_state(s->gridPipeline, VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, NULL, VK_FALSE, VK_FALSE);

	vkh_pipeline_set_depth_stencil_state(s->gridPipeline, VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS, VK_FALSE, VK_FALSE, {}, {}, 0.0f, 1.0f);

	vkh_pipeline_set_color_blend_state(s->gridPipeline, VK_FALSE, VK_LOGIC_OP_COPY, 0.0f, 0.0f, 0.0f, 0.0f);

	//generate pipeline:
	//---------------
	if(!vkh_pipeline_generate(s->gridPipeline, s->instance, s->finalRenderPass, 0))
		return false;

	//cleanup:
	//---------------
	vkh_free_spirv(vertCode);
	vkh_free_spirv(fragCode);

	vkh_destroy_shader_module(s->instance, vertModule);
	vkh_destroy_shader_module(s->instance, fragModule);

	return true;
}
void vkh_pipeline_cleanup(VKHgraphicsPipeline* pipeline, VKHinstance* inst)
{
	if(!pipeline->generated)
		return;

	vkDestroyPipeline           (inst->device, pipeline->pipeline, NULL);
	vkDestroyPipelineLayout     (inst->device, pipeline->layout, NULL);
	vkDestroyDescriptorSetLayout(inst->device, pipeline->descriptorLayout, NULL);

	pipeline->generated = VKH_FALSE;
}
void _draw_destroy_grid_pipeline(DrawState* s)
{
	vkh_pipeline_cleanup(s->gridPipeline, s->instance);
	vkh_pipeline_destroy(s->gridPipeline);
}
void _draw_destroy_quad_pipeline(DrawState* s)
{
	vkh_pipeline_cleanup(s->gridPipeline, s->instance);
	vkh_pipeline_destroy(s->gridPipeline);
}
void _draw_destroy_model1_pipeline(DrawState* s)
{
	vkh_pipeline_cleanup(s->gridPipeline, s->instance);
	vkh_pipeline_destroy(s->gridPipeline);
}
void _draw_destroy_quadTexture1_pipeline(DrawState* s)
{
	vkh_pipeline_cleanup(s->quadTexture1Pipeline, s->instance);
	vkh_pipeline_destroy(s->quadTexture1Pipeline);
}
//----------------------------------------------------------------------------//
VKHdescriptorSets* vkh_descriptor_sets_create(uint32_t count)
{
	VKHdescriptorSets* descriptorSets = (VKHdescriptorSets*)malloc(sizeof(VKHdescriptorSets));
	if(!descriptorSets)
		return NULL;

	descriptorSets->count = count;

	descriptorSets->descriptors = MakeDynamicArray<VKHdescriptorInfo>(&Zayn->permanentMemArena, 20);

	descriptorSets->generated = VKH_FALSE;
	descriptorSets->pool = VK_NULL_HANDLE;
	descriptorSets->sets = (VkDescriptorSet*)malloc(descriptorSets->count * sizeof(VkDescriptorSet));

	return descriptorSets;
}
void vkh_descriptor_sets_destroy(VKHdescriptorSets* descriptorSets)
{
	if(descriptorSets->generated)
	{
		ERROR_LOG("you must call vkh_descriptor_sets_cleanup() before calling vkh_descriptor_sets_destroy()");
		return;
	}

	DynamicArrayClear(&descriptorSets->descriptors);
	free(descriptorSets->sets);

	free(descriptorSets);
}
void vkh_descriptor_sets_add_buffers(VKHdescriptorSets* descriptorSets, uint32_t index, VkDescriptorType type, uint32_t binding, uint32_t arrayElem, 
                                     uint32_t count, VkDescriptorBufferInfo* bufferInfos)
{
	VKHdescriptorInfo info;
	info.index = index;
	info.type = type;
	info.binding = binding;
	info.arrayElem = arrayElem;
	info.count = count;
	info.bufferInfos = bufferInfos;

	PushBack(&descriptorSets->descriptors, info);
}

void vkh_descriptor_sets_add_images(VKHdescriptorSets* descriptorSets, uint32_t index, VkDescriptorType type, 
                                    uint32_t binding, uint32_t arrayElement, uint32_t count, VkDescriptorImageInfo* imageInfos)
{
    // Ensure the descriptorSets and imageInfos pointers are valid
    assert(descriptorSets);
    assert(imageInfos);

    // Create a new descriptor info entry
    VKHdescriptorInfo descriptorInfo = {};
    descriptorInfo.type = type;
    descriptorInfo.binding = binding;
    descriptorInfo.arrayElem = arrayElement;
    descriptorInfo.count = count;
    descriptorInfo.index = index;
    descriptorInfo.imageInfos = imageInfos;

    // Add the descriptor info to the array of descriptors
    PushBack(&descriptorSets->descriptors, descriptorInfo);
}

vkh_bool_t vkh_desctiptor_sets_generate(VKHdescriptorSets* descriptorSets, VKHinstance* inst, VkDescriptorSetLayout layout)
{
	if(descriptorSets->generated)
	{
		ERROR_LOG("descriptor sets have already been generated, call vkh_descriptor_sets_cleanup() before generating again");
		return VKH_FALSE;
	}

	//create pool:
	//---------------
	DynamicArray<VkDescriptorPoolSize> poolSizes = MakeDynamicArray<VkDescriptorPoolSize>(&Zayn->permanentMemArena, descriptorSets->count);

	for(size_t i = 0; i < descriptorSets->descriptors.count; i++)
	{
		VKHdescriptorInfo info = (VKHdescriptorInfo)descriptorSets->descriptors[i];
		VkDescriptorType type = info.type;

		vkh_bool_t found = VKH_FALSE;
		for(size_t j = 0; j < poolSizes.count; j++)
		{
			VkDescriptorPoolSize *poolSize = &poolSizes[j];
			if(poolSize->type == type)
			{
				// ((VkDescriptorPoolSize*)qd_dynarray_get(poolSizes, j))->descriptorCount++;
				poolSize->descriptorCount++;
				found = VKH_TRUE;
				break;
			}
		}

		if(!found)
		{
			VkDescriptorPoolSize poolSize = {};
			poolSize.type = type;
			poolSize.descriptorCount = 1;

			PushBack(&poolSizes, poolSize);
		}
	}

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = (uint32_t)poolSizes.count;
	poolInfo.pPoolSizes = &poolSizes[0];
	poolInfo.maxSets = descriptorSets->count;

	if(vkCreateDescriptorPool(inst->device, &poolInfo, NULL, &descriptorSets->pool) != VK_SUCCESS)
	{
		ERROR_LOG("failed to create descriptor pool");
		return VKH_FALSE;
	}

	//create sets:
	//---------------
	VkDescriptorSetLayout* layouts = (VkDescriptorSetLayout*)malloc(descriptorSets->count * sizeof(VkDescriptorSetLayout));
	for(uint32_t i = 0; i < descriptorSets->count; i++)
		layouts[i] = layout;

	VkDescriptorSetAllocateInfo setAllocInfo = {};
	setAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	setAllocInfo.descriptorPool = descriptorSets->pool;
	setAllocInfo.descriptorSetCount = descriptorSets->count;
	setAllocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(inst->device, &setAllocInfo, descriptorSets->sets) != VK_SUCCESS)
	{
		ERROR_LOG("failed to allocate descriptor sets");
		return VKH_FALSE;
	}

	//write descriptors:
	//---------------
	VkWriteDescriptorSet* writes = (VkWriteDescriptorSet*)calloc(descriptorSets->descriptors.count, sizeof(VkWriteDescriptorSet));

	for(size_t i = 0; i < descriptorSets->descriptors.count; i++)
	{
		VKHdescriptorInfo* info = (VKHdescriptorInfo*)&descriptorSets->descriptors[i];

		writes[i].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[i].dstSet = descriptorSets->sets[info->index];
		writes[i].dstBinding = info->binding;
		writes[i].dstArrayElement = info->arrayElem;
		writes[i].descriptorType = info->type;
		writes[i].descriptorCount = info->count;
		writes[i].pBufferInfo = info->bufferInfos;
		writes[i].pImageInfo = info->imageInfos;
		writes[i].pTexelBufferView = info->texelBufferViews;
	}

	vkUpdateDescriptorSets(inst->device, (uint32_t)descriptorSets->descriptors.count, writes, 0, NULL);

	//cleanup:
	//---------------
	DynamicArrayClear(&poolSizes);
	free(layouts);
	free(writes);

	descriptorSets->generated = VKH_TRUE;
	return VKH_TRUE;
}
void vkh_descriptor_sets_cleanup(VKHdescriptorSets* descriptorSets, VKHinstance* inst)
{
	if(!descriptorSets->generated)
		return;

	vkDestroyDescriptorPool(inst->device, descriptorSets->pool, NULL);
	descriptorSets->generated = VKH_FALSE;
}
bool _draw_create_quad_descriptors(DrawState* s)
{
	s->gridDescriptorSets = vkh_descriptor_sets_create(FRAMES_IN_FLIGHT);
	if(!s->gridDescriptorSets)
		return false;

	VkDescriptorBufferInfo cameraBufferInfos[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		cameraBufferInfos[i].buffer = s->cameraBuffers[i];
		cameraBufferInfos[i].offset = 0;
		cameraBufferInfos[i].range = sizeof(CameraGPU);

		vkh_descriptor_sets_add_buffers(s->gridDescriptorSets, i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
			0, 0, 1, &cameraBufferInfos[i]);
	}

	return vkh_desctiptor_sets_generate(s->gridDescriptorSets, s->instance, s->gridPipeline->descriptorLayout);
}
static void _draw_destroy_quad_descriptors(DrawState* s)
{
	vkh_descriptor_sets_cleanup(s->gridDescriptorSets, s->instance);
	vkh_descriptor_sets_destroy(s->gridDescriptorSets);
}
bool _draw_create_model1_descriptors(DrawState* s)
{
	s->model1DescriptorSets = vkh_descriptor_sets_create(FRAMES_IN_FLIGHT);
	if(!s->model1DescriptorSets)
		return false;

	VkDescriptorBufferInfo cameraBufferInfos[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		cameraBufferInfos[i].buffer = s->cameraBuffers[i];
		cameraBufferInfos[i].offset = 0;
		cameraBufferInfos[i].range = sizeof(CameraGPU);

		vkh_descriptor_sets_add_buffers(s->model1DescriptorSets, i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
			0, 0, 1, &cameraBufferInfos[i]);
	}
	VkDescriptorImageInfo imageInfo[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo[i].imageView = s->model1TextureImageView;
		imageInfo[i].sampler = s->model1TextureSampler;
	}

	return vkh_desctiptor_sets_generate(s->model1DescriptorSets, s->instance, s->model1Pipeline->descriptorLayout);
}
static void _draw_destroy_model1_descriptors(DrawState* s)
{
	vkh_descriptor_sets_cleanup(s->gridDescriptorSets, s->instance);
	vkh_descriptor_sets_destroy(s->gridDescriptorSets);
}
bool _draw_create_quadTexture1_descriptors(DrawState* s)
{
	s->quadTexture1DescriptorSets = vkh_descriptor_sets_create(FRAMES_IN_FLIGHT);
	if(!s->quadTexture1DescriptorSets)
		return false;

	VkDescriptorBufferInfo cameraBufferInfos[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		cameraBufferInfos[i].buffer = s->cameraBuffers[i];
		cameraBufferInfos[i].offset = 0;
		cameraBufferInfos[i].range = sizeof(CameraGPU);

		vkh_descriptor_sets_add_buffers(s->quadTexture1DescriptorSets, i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
			0, 0, 1, &cameraBufferInfos[i]);
	}
	VkDescriptorImageInfo imageInfo[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		imageInfo[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo[i].imageView = s->quadTexture1TextureImageView;
		imageInfo[i].sampler = s->quadTexture1TextureSampler;

		vkh_descriptor_sets_add_images(s->quadTexture1DescriptorSets, i, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 
            1, 0, 1, &imageInfo[i]);
	}

	return vkh_desctiptor_sets_generate(s->quadTexture1DescriptorSets, s->instance, s->quadTexture1Pipeline->descriptorLayout);
}
static void _draw_destroy_quadTexture1_descriptors(DrawState* s)
{
	vkh_descriptor_sets_cleanup(s->quadTexture1DescriptorSets, s->instance);
	vkh_descriptor_sets_destroy(s->quadTexture1DescriptorSets);
}
bool _draw_create_grid_descriptors(DrawState* s)
{
	s->gridDescriptorSets = vkh_descriptor_sets_create(FRAMES_IN_FLIGHT);
	if(!s->gridDescriptorSets)
		return false;

	VkDescriptorBufferInfo cameraBufferInfos[FRAMES_IN_FLIGHT];
	for(int32 i = 0; i < FRAMES_IN_FLIGHT; i++)
	{
		cameraBufferInfos[i].buffer = s->cameraBuffers[i];
		cameraBufferInfos[i].offset = 0;
		cameraBufferInfos[i].range = sizeof(CameraGPU);

		vkh_descriptor_sets_add_buffers(s->gridDescriptorSets, i, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 
			0, 0, 1, &cameraBufferInfos[i]);
	}

	return vkh_desctiptor_sets_generate(s->gridDescriptorSets, s->instance, s->gridPipeline->descriptorLayout);
}
static void _draw_destroy_grid_descriptors(DrawState* s)
{
	vkh_descriptor_sets_cleanup(s->gridDescriptorSets, s->instance);
	vkh_descriptor_sets_destroy(s->gridDescriptorSets);
}
//----------------------------------------------------------------------------//
void vkh_resize_swapchain(VKHinstance* inst, uint32_t w, uint32_t h)
{
	if(w == 0 || h == 0) //TODO: test
		return;
	
	vkDeviceWaitIdle(inst->device);

	_vkh_destroy_swapchain(inst);
	_vkh_create_swapchain(inst, w, h);
}
//----------------------------------------------------------------------------//
void _draw_window_resized(DrawState* s)
{
	int32 w, h;
	glfwGetFramebufferSize(s->instance->window, &w, &h);
	if(w == 0 || h == 0)
		return;

	vkh_resize_swapchain(s->instance, w, h);

	_draw_destroy_depth_buffer(s);
	_draw_create_depth_buffer(s);

	_draw_destroy_framebuffers(s);
	_draw_create_framebuffers(s);
}
void _draw_record_render_pass_start_commands(DrawState* s, VkCommandBuffer commandBuffer, uint32 frameIndex, uint32 imageIdx)
{
	//render pass begin:
	//---------------
	VkRenderPassBeginInfo renderBeginInfo = {};
	renderBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderBeginInfo.renderPass = s->finalRenderPass;
	renderBeginInfo.framebuffer = s->framebuffers[imageIdx];
	renderBeginInfo.renderArea.offset = {0, 0};
	renderBeginInfo.renderArea.extent = s->instance->swapchainExtent;

	VkClearValue clearValues[3];
	clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
	clearValues[1].depthStencil = {1.0f, 0};
	clearValues[2].color = {{0.0f, 0.0f, 0.0f, 1.0f}};

	renderBeginInfo.clearValueCount = 3;
	renderBeginInfo.pClearValues = clearValues;

	vkCmdBeginRenderPass(commandBuffer, &renderBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	//set viewport and scissor:
	//---------------
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = (f32)s->instance->swapchainExtent.height;
	viewport.width = (f32)s->instance->swapchainExtent.width;
	viewport.height = -(f32)s->instance->swapchainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = s->instance->swapchainExtent;
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}
void _draw_record_quad_commands(DrawState* s, DrawParams* params, VkCommandBuffer commandBuffer, uint32 frameIndex, uint32 imageIdx)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->gridPipeline->pipeline);

	//bind buffers:
	//---------------
	VkBuffer vertexBuffers[] = {s->quadVertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, s->quadIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->gridPipeline->layout, 0, 1, &s->gridDescriptorSets->sets[frameIndex], 0, nullptr);

	int32 numCells = 16;

	//send vertex stage params:
	//---------------
	int32 windowW, windowH;
	glfwGetWindowSize(s->instance->window, &windowW, &windowH);	//TODO: FIGURE OUT WHY IT GETS CUT OFF WITH VERY TALL WINDOWS
	f32 aspect = (f32)windowW / (f32)windowH;
	if(aspect < 1.0f)
		aspect = 1.0f / aspect;

	f32 size = aspect * powf(2.0f, roundf(log2f(params->cam.dist) + 0.5f));

	vec3 pos = params->cam.target;
	for(int32 i = 0; i < 3; i++)
		pos[i] -= fmodf(pos[i], size / numCells);

	mat4 model = translate(pos) * scale(V3(size, size, size));

	GridParamsVertGPU vertParams = {model};
	vkCmdPushConstants(commandBuffer, s->gridPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GridParamsVertGPU), &vertParams);

	//send fragment stage params:
	//---------------
	f32 thickness = 0.125f;
	f32 scroll = (params->cam.dist - powf(2.0f, roundf(log2f(params->cam.dist) - 0.5f))) / (4.0f * powf(2.0f, roundf(log2f(params->cam.dist) - 1.5f))) + 0.5f;
	vec3 offset3 = (params->cam.target - pos) / size;
	vec2 offset = V2(offset3.x, offset3.z);

	GridParamsFragGPU fragParams = {offset, numCells, thickness,scroll};
	// GridParamsFragGPU fragParams = {offset, numCells, thickness, scroll};
	vkCmdPushConstants(commandBuffer, s->gridPipeline->layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(GridParamsVertGPU), sizeof(GridParamsFragGPU), &fragParams);

	//draw:
	//---------------
	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
}
void _draw_record_quadTexture1_commands(DrawState* s, DrawParams* params, VkCommandBuffer commandBuffer, uint32 frameIndex, uint32 imageIdx)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->quadTexture1Pipeline->pipeline);

	//bind buffers:
	//---------------
	VkBuffer vertexBuffers[] = {s->quadTexture1VertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, s->quadTexture1IndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->quadTexture1Pipeline->layout, 0, 1, &s->quadTexture1DescriptorSets->sets[frameIndex], 0, nullptr);

	// int32 numCells = 16;

	//send vertex stage params:
	//---------------
	int32 windowW, windowH;
	glfwGetWindowSize(s->instance->window, &windowW, &windowH);	//TODO: FIGURE OUT WHY IT GETS CUT OFF WITH VERY TALL WINDOWS
	f32 aspect = (f32)windowW / (f32)windowH;
	if(aspect < 1.0f)
		aspect = 1.0f / aspect;

	f32 size = aspect * powf(2.0f, roundf(log2f(params->cam.dist) + 0.5f));

	vec3 pos = V3(0);
	// vec3 pos = params->cam.target;
	for(int32 i = 0; i < 3; i++)
		// pos[i] -= fmodf(pos[i], size / numCells);

	mat4 model1 = translate(pos) * scale(V3(size, size, size));
	// mat4 model1 = translate(pos) * scale(V3(size, size, size));

	GridParamsVertGPU vertParams = {};
	vertParams.model = translate(pos) * scale(V3(size, size, size));
	vkCmdPushConstants(commandBuffer, s->quadTexture1Pipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GridParamsVertGPU), &vertParams);

	//send fragment stage params:
	//---------------
	// f32 thickness = 0.125f;
	// f32 scroll = (params->cam.dist - powf(2.0f, roundf(log2f(params->cam.dist) - 0.5f))) / (4.0f * powf(2.0f, roundf(log2f(params->cam.dist) - 1.5f))) + 0.5f;
	// vec3 offset3 = (params->cam.target - pos) / size;
	// vec2 offset = V2(offset3.x, offset3.z);

	// GridParamsFragGPU fragParams = {offset, numCells, thickness,scroll};
	// GridParamsFragGPU fragParams = {offset, numCells, thickness, scroll};
	// vkCmdPushConstants(commandBuffer, s->gridPipeline->layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(GridParamsVertGPU), sizeof(GridParamsFragGPU), &fragParams);

	//draw:
	//---------------
	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
}
//----------------------------------------------------------------------------//
void _draw_record_model1_commands(DrawState* s, DrawParams* params, VkCommandBuffer commandBuffer, uint32 frameIndex, uint32 imageIdx)
{


}
void _draw_record_grid_commands(DrawState* s, DrawParams* params, VkCommandBuffer commandBuffer, uint32 frameIndex, uint32 imageIdx)
{
	vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->gridPipeline->pipeline);

	//bind buffers:
	//---------------
	VkBuffer vertexBuffers[] = {s->quadVertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, s->quadIndexBuffer, 0, VK_INDEX_TYPE_UINT32);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->gridPipeline->layout, 0, 1, &s->gridDescriptorSets->sets[frameIndex], 0, nullptr);

	int32 numCells = 16;

	//send vertex stage params:
	//---------------
	int32 windowW, windowH;
	glfwGetWindowSize(s->instance->window, &windowW, &windowH);	//TODO: FIGURE OUT WHY IT GETS CUT OFF WITH VERY TALL WINDOWS
	f32 aspect = (f32)windowW / (f32)windowH;
	if(aspect < 1.0f)
		aspect = 1.0f / aspect;

	f32 size = aspect * powf(2.0f, roundf(log2f(params->cam.dist) + 0.5f));

	vec3 pos = params->cam.target;
	for(int32 i = 0; i < 3; i++)
		pos[i] -= fmodf(pos[i], size / numCells);

	mat4 model = translate(pos) * scale(V3(size, size, size));

	GridParamsVertGPU vertParams = {model};
	vkCmdPushConstants(commandBuffer, s->gridPipeline->layout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(GridParamsVertGPU), &vertParams);

	//send fragment stage params:
	//---------------
	f32 thickness = 0.0125f;
	f32 scroll = (params->cam.dist - powf(2.0f, roundf(log2f(params->cam.dist) - 0.5f))) / (4.0f * powf(2.0f, roundf(log2f(params->cam.dist) - 1.5f))) + 0.5f;
	vec3 offset3 = (params->cam.target - pos) / size;
	vec2 offset = V2(offset3.x, offset3.z);

	// GridParamsFragGPU fragParams = {};
	GridParamsFragGPU fragParams = {offset, numCells, thickness, scroll};
	vkCmdPushConstants(commandBuffer, s->gridPipeline->layout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(GridParamsVertGPU), sizeof(GridParamsFragGPU), &fragParams);

	//draw:
	//---------------
	vkCmdDrawIndexed(commandBuffer, 6, 1, 0, 0, 0);
}
//----------------------------------------------------------------------------//
void draw_render(DrawState* s, DrawParams* params, f32 dt)
{
	static uint32 frameIdx = 0;
	//wait for fences and get next swapchain image: (essentially just making sure last frame is done):
	//---------------
	vkWaitForFences(s->instance->device, 1, &s->inFlightFences[frameIdx], VK_TRUE, UINT64_MAX);

	uint32 imageIdx;
	VkResult imageAquireResult = vkAcquireNextImageKHR(s->instance->device, s->instance->swapchain, UINT64_MAX,
													   s->imageAvailableSemaphores[frameIdx], VK_NULL_HANDLE, &imageIdx);
	if(imageAquireResult == VK_ERROR_OUT_OF_DATE_KHR || imageAquireResult == VK_SUBOPTIMAL_KHR)
	{
		_draw_window_resized(s);
		return;
	}
	else if(imageAquireResult != VK_SUCCESS)
	{
		ERROR_LOG("failed to acquire swapchain image");
		return;
	}

	vkResetFences(s->instance->device, 1, &s->inFlightFences[frameIdx]);
	Camera* cam = &Zayn->camera;

	// if (InputHeld(Keyboard, Input_A))
	// {
	// 	cam->pos.x -= 0.1f * Zayn->deltaTime; 
	// }
	// if (InputHeld(Keyboard, Input_D))
	// {
	// 	cam->pos.x += 0.1f * Zayn->deltaTime; 
	// }
	// if (InputHeld(Keyboard, Input_W))
	// {
	// 	cam->pos.y -= 0.1f * Zayn->deltaTime; 
	// }
	// if (InputHeld(Keyboard, Input_S))
	// {
	// 	cam->pos.y += 0.1f * Zayn->deltaTime; 
	// }
	//update camera buffer:
	//---------------
	int32 windowW, windowH;
	glfwGetWindowSize(s->instance->window, &windowW, &windowH);

	mat4 view, projection;
	view = lookat(params->cam.pos, params->cam.target, params->cam.up);
	projection = perspective(params->cam.fov, (f32)windowW / (f32)windowH, 0.1f, INFINITY);

    UniformBufferObject ubo{};
    ubo.model = TRS(V3(0.0f, 0.0f, 0.0f), AxisAngle(V3(0.0f, 1.0f, 0.0f), /*time * */ DegToRad(0.0f)), V3(1.0f, 1.0f, 1.0f));
    
    ubo.view = glm::lookAt(cam->pos, cam->pos + cam->front, cam->up );
    
    ubo.proj = glm::perspective(glm::radians(60.0f), (f32)windowW / (f32)windowH, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;


	CameraGPU camBuffer;
	camBuffer.view = ubo.view;
	camBuffer.proj = ubo.proj;
    // ubo.viewProj = Zayn->camera.viewProjection;

	std::cout << "Game Camera Position: " << camBuffer.view[3][0] << ", " << camBuffer.view[3][1] << ", " << camBuffer.view[3][2] << std::endl;

	camBuffer.viewProj = ubo.proj * ubo.view;
	vkh_copy_with_staging_buf(s->instance, s->cameraStagingBuffer, s->cameraStagingBufferMemory,
									  s->cameraBuffers[frameIdx], sizeof(CameraGPU), 0, &camBuffer);

	// //start command buffer:
	// //---------------
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0;
	beginInfo.pInheritanceInfo = nullptr;

	vkResetCommandBuffer(s->commandBuffers[frameIdx], 0);
	vkBeginCommandBuffer(s->commandBuffers[frameIdx], &beginInfo);

	// //record commands:
	// //---------------
	_draw_record_render_pass_start_commands(s, s->commandBuffers[frameIdx], frameIdx, imageIdx);

	// _draw_record_grid_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);
	_draw_record_quadTexture1_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);
	// // _draw_record_model1_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);
	// // _draw_record_triangle_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);
	// _draw_record_quad_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);
	// // _draw_record_particle_commands(s, params, s->commandBuffers[frameIdx], frameIdx, imageIdx);

	// //end command buffer:
	// //---------------
	vkCmdEndRenderPass(s->commandBuffers[frameIdx]);

	if(vkEndCommandBuffer(s->commandBuffers[frameIdx]) != VK_SUCCESS)
		ERROR_LOG("failed to end command buffer");

	// //submit command buffer:
	// //---------------
	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = &s->imageAvailableSemaphores[frameIdx];
	submitInfo.pWaitDstStageMask = &waitStage;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &s->commandBuffers[frameIdx];
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = &s->renderFinishedSemaphores[frameIdx];

	vkQueueSubmit(s->instance->graphicsQueue, 1, &submitInfo, s->inFlightFences[frameIdx]);

	// //present to screen:
	// //---------------
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &s->renderFinishedSemaphores[frameIdx];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &s->instance->swapchain;
	presentInfo.pImageIndices = &imageIdx;
	presentInfo.pResults = nullptr;

	VkResult presentResult = vkQueuePresentKHR(s->instance->presentQueue, &presentInfo);
	if(presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR)
		_draw_window_resized(s);
	else if(presentResult != VK_SUCCESS)
		ERROR_LOG("failed to present swapchain image");

	frameIdx = (frameIdx + 1) % FRAMES_IN_FLIGHT;
}
template<typename T>
void _game_decay_to(T& value, T target, f32 rate, f32 dt)
{
	value = value + (target - value) * (1.0f - powf(rate, 1000.0f * dt));
}
//----------------------------------------------------------------------------//
void _game_camera_update
(GameCamera* cam, f32 dt, GLFWwindow* window)
{
	// f32 camSpeed = 1.0f * dt * cam->dist;
	// f32 angleSpeed = 45.0f * dt;
	// f32 tiltSpeed = 30.0f * dt;

	// vec4 forward4 = rotate(cam->up, cam->angle) * V4(0.0f, 0.0f, -1.0f, 1.0f);
	// vec4 side4 = rotate(cam->up, cam->angle) * V4(1.0f, 0.0f, 0.0f, 1.0f);

	// vec3 forward = V3(forward4.x, forward4.y, forward4.z);
	// vec3 side = V3(side4.x, side4.y, side4.z);

	// vec3 camVel = V3(0.0f, 0.0f, 0.0f);
	// if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	// 	camVel = camVel + forward;
	// if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	camVel = camVel - forward;
	// if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	// 	camVel = camVel + side;
	// if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	// 	camVel = camVel - side;

	// if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	// 	cam->targetAngle -= angleSpeed;
	// if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	// 	cam->targetAngle += angleSpeed;

	// if(glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	// 	cam->targetTilt = fminf(cam->targetTilt + tiltSpeed, CAMERA_MAX_TILT);
	// if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	// 	cam->targetTilt = fmaxf(cam->targetTilt - tiltSpeed, CAMERA_MIN_TILT);

	// cam->targetCenter = cam->targetCenter + camSpeed * normalize(camVel);
	// if(length(cam->targetCenter) > CAMERA_MAX_POSITION)
	// 	cam->targetCenter = normalize(cam->targetCenter) * CAMERA_MAX_POSITION;

	// _game_decay_to(cam->center, cam->targetCenter, 0.985f, dt);
	// _game_decay_to(cam->dist  , cam->targetDist  , 0.99f , dt);
	// _game_decay_to(cam->angle , cam->targetAngle , 0.99f , dt);
	// _game_decay_to(cam->tilt  , cam->targetTilt  , 0.99f , dt);

	// vec4 toPos = rotate(side, -cam->tilt) * V4(forward, 1.0f);
	// cam->pos = cam->center - cam->dist * normalize(V3(toPos.x, toPos.y, toPos.z));


	// Camera* cam = &Zayn->camera;

    // UniformBufferObject_x ubo{};
    // ubo.model = TRS(V3(0.0f, 0.0f, 0.0f), AxisAngle(V3(0.0f, 1.0f, 0.0f), time * DegToRad(0.0f)), V3(1.0f, 1.0f, 1.0f));
    
    // ubo.view = glm::lookAt(cam->pos, cam->pos + cam->front, cam->up );
    
    // ubo.proj = glm::perspective(glm::radians(60.0f), zaynMem->vkSwapChainExtent.width / (float)zaynMem->vkSwapChainExtent.height, 0.1f, 10.0f);
    // ubo.proj[1][1] *= -1;
    // ubo.viewProj = zaynMem->camera.viewProjection;
}
//----------------------------------------------------------------------------//
bool _game_camera_init(GameCamera* cam)
{
	if(!cam)
	{
		ERROR_LOG("failed to allocate GameCamera struct");
		return false;
	}

	cam->up = {0.0f, 1.0f, 0.0f};
	cam->center = cam->targetCenter = {0.0f, 0.0f, 0.0f};

	cam->dist = cam->targetDist = CAMERA_MAX_DIST;
	cam->angle = cam->targetAngle = 45.0f;
	cam->tilt = cam->targetTilt = 45.0f;

	return true;
}
bool draw_init()
{
    Zayn->drawState = (DrawState* )malloc(sizeof(DrawState));
	DrawState* draw = Zayn->drawState;
    // if (!vkh_init(&Zayn->vkInstance, 1920, 1080, "VkGalaxy"))
	if(!draw)
	{
		ERROR_LOG("failed to allocate GameState struct");
		return false;
	}

    if (!vkh_init(&draw->instance))
    {
        ERROR_LOG("failed to initialize render instance");
        return false;
    }

	//initialize objects for drawing:
	//---------------
	if(!_draw_create_depth_buffer(draw))
	{
		return false;
	}
	if(!_draw_create_final_render_pass(draw))
	{
		return false;
	}
	if(!_draw_create_framebuffers(draw))
	{
		return false;	
	}	
	if(!_draw_create_command_buffers(draw))
	{
		return false;
	}
	if(!_draw_create_sync_objects(draw))
	{
		return false;
	}
	if(!_draw_create_camera_buffer(draw))
	{
		return false;
	}

	// init and load models
	// if (!_draw_create_model1_load_model(draw, "/Users/socki/dev/zayn/models/textures/viking_room.png", "/Users/socki/dev/zayn/models/viking_room.obj"))
	// {
	// 	return false;
	// }

	// //initialize reusable vertex buffers:
	// //---------------
	// if(!_draw_create_quad_vertex_buffer(draw))
	// {
	// 	return false;
	// }
	if(!_draw_create_quadTexture1_vertex_buffer(draw, "/Users/socki/dev/zayn/models/textures/texture_test.jpg"))
	{
		return false;
	}


	// //initialize grid drawing objects:
	// //---------------
	// if(!_draw_create_grid_pipeline(draw))
	// {
	// 	return false;
	// }
	// if(!_draw_create_grid_descriptors(draw))
	// {
	// 	return false;
	// }
	// if(!_draw_create_model1_pipeline(draw))
	// {
	// 	return false;
	// }
	// if(!_draw_create_model1_descriptors(draw))
	// {
	// 	return false;
	// }
	if(!_draw_create_quadTexture1_pipeline(draw))
	{
		return false;
	}
	if(!_draw_create_quadTexture1_descriptors(draw))
	{
		return false;
	}



	return true;
}
void InitRender_Learn()
{
	DrawState* draw = Zayn->drawState;

    draw_init();

    if(!_game_camera_init(&Zayn->gameCamera))
	{
		ERROR_LOG("failed to initialize camera");
		// return false;
	}
}
void UpdateRender_Learn()
{
	// DrawState* draw = Zayn->drawState;
	// f32 curTime = (f32)glfwGetTime();
	// f32 dt = curTime - lastTime;
	// lastTime = curTime;
	Zayn->accumTime += Zayn->deltaTime;
	Zayn->accumFrames++;
	if (Zayn->accumTime >= 1.0f)
	{
		float avgDt = Zayn->accumTime / Zayn->accumFrames;

		char windowName[64];
		snprintf(windowName, sizeof(windowName), "VkGalaxy [FPS: %.0f (%.2fms)]", 1.0f / avgDt, avgDt * 1000.0f);
		glfwSetWindowTitle(Zayn->drawState->instance->window, windowName);

		Zayn->accumTime -= 1.0f;
		Zayn->accumFrames = 0;
	}

	_game_camera_update(&Zayn->gameCamera, Zayn->deltaTime, Zayn->drawState->instance->window);
	
	DrawParams drawParams;
	drawParams.cam.pos = Zayn->gameCamera.pos;
	drawParams.cam.up = Zayn->gameCamera.up;
	drawParams.cam.target = Zayn->gameCamera.center;
	drawParams.cam.dist = Zayn->gameCamera.dist;
	drawParams.cam.fov = CAMERA_FOV;
	draw_render(Zayn->drawState, &drawParams, Zayn->deltaTime);
}

void RenderCleanup()
{
	vkDeviceWaitIdle(Zayn->drawState->instance->device);

	// _draw_destroy_particle_descriptors(Zayn->drawState);
	// _draw_destroy_particle_buffer(Zayn->drawState);
	// _draw_destroy_particle_pipeline(Zayn->drawState);

	_draw_destroy_grid_descriptors(Zayn->drawState);
	_draw_destroy_grid_pipeline(Zayn->drawState);

	_draw_destroy_quad_vertex_buffer(Zayn->drawState);

	_draw_destroy_camera_buffer(Zayn->drawState);
	_draw_destroy_sync_objects(Zayn->drawState);
	_draw_destroy_command_buffers(Zayn->drawState);
	_draw_destroy_framebuffers(Zayn->drawState);
	_draw_destroy_final_render_pass(Zayn->drawState);
	_draw_destroy_depth_buffer(Zayn->drawState);


	_vkh_destroy_command_pool(Zayn->drawState->instance);
	_vkh_destroy_swapchain(Zayn->drawState->instance);
	_vkh_destroy_vk_device(Zayn->drawState->instance);
	_vkh_destroy_vk_instance(Zayn->drawState->instance);
	// _vkh_quit_glfw(&Zayn->drawState->instance);				// handled in main.cpp
	free(Zayn->drawState->instance);
	free(Zayn->drawState);

}