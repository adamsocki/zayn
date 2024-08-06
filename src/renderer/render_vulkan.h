typedef int32_t vkh_bool_t;
#define VKH_TRUE  1
#define VKH_FALSE 0

#define FRAMES_IN_FLIGHT 2
#define CAMERA_FOV 45.0f

#define CAMERA_MAX_DIST 8000.0f
#define CAMERA_MIN_TILT 15.0f
#define CAMERA_MAX_TILT 89.0f
#define CAMERA_MAX_POSITION 7000.0f
struct Vertex_NEW
{
	vec3 pos;
	vec2 texCoord;
};

//----------------------------------------------------------------------------//

// mirrors camera buffer on GPU
struct CameraGPU
{
	mat4 view;
	mat4 proj;
	mat4 viewProj;
};

//----------------------------------------------------------------------------//

//parameters for grid rendering
struct GridParamsVertGPU
{
	mat4 model;
};

//parameters for grid rendering
struct GridParamsFragGPU
{
	vec2 offset;
	int32 numCells;
	real32 thickness;
	real32 scroll;
};

struct DrawParams
{
	struct
	{
		vec3 pos;
		vec3 up;
		vec3 target;

		real32 dist;

		real32 fov;
	} cam;
};

struct GameCamera
{
	vec3 pos;
	vec3 up;
	vec3 center;
	vec3 targetCenter;

	real32 dist;
	real32 targetDist;

	real32 tilt;
	real32 targetTilt;

	real32 angle;
	real32 targetAngle;
};

struct VKHinstance
{
	GLFWwindow* window;

	VkInstance instance;
	VkDevice device;
	VkSurfaceKHR surface;
	VkPhysicalDevice physicalDevice;

	uint32_t graphicsComputeFamilyIdx;
	uint32_t presentFamilyIdx;
	VkQueue graphicsQueue;
	VkQueue computeQueue;
	VkQueue presentQueue;

	VkSwapchainKHR swapchain;
	VkFormat swapchainFormat;
	VkExtent2D swapchainExtent;
	uint32_t swapchainImageCount;
	VkImage* swapchainImages;
	VkImageView* swapchainImageViews;

	VkCommandPool commandPool;

	#if VKH_VALIDATION_LAYERS
		VkDebugUtilsMessengerEXT debugMessenger;
	#endif
};
 
 struct VKHgraphicsPipeline
{
	//intermediates:
	//---------------
	DynamicArray<VkDescriptorSetLayoutBinding> descSetBindings;                 //type - VkDescriptorSetLayoutBinding
	DynamicArray<VkDynamicState> dynamicStates;                                 //type - VkDynamicState
	DynamicArray<VkVertexInputBindingDescription> vertInputBindings;            //type - VkVertexInputBindingDescription
	DynamicArray<VkVertexInputAttributeDescription> vertInputAttribs;           //type - VkVertexInputAttributeDescription
	DynamicArray<VkViewport> viewports;                                         //type - VkViewport
	DynamicArray<VkRect2D> scissors;                                            //type - VkRect2D
	DynamicArray<VkPipelineColorBlendAttachmentState> colorBlendAttachments;    //type - VkPipelineColorBlendAttachmentState
	DynamicArray<VkPushConstantRange> pushConstants;                            //type - VkPushConstantRange

	VkShaderModule vertShader;
	VkShaderModule fragShader;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyState;
	VkPipelineTessellationStateCreateInfo tesselationState;
	VkPipelineRasterizationStateCreateInfo rasterState;
	VkPipelineMultisampleStateCreateInfo multisampleState;
	VkPipelineDepthStencilStateCreateInfo depthStencilState;
	VkPipelineColorBlendStateCreateInfo colorBlendState;

	//generated:
	//---------------
	vkh_bool_t generated;

	VkDescriptorSetLayout descriptorLayout;
	VkPipelineLayout layout;
	VkPipeline pipeline;

};

struct VKHdescriptorInfo
{
	uint32_t index;

	uint32_t count;
	union
	{
		VkDescriptorBufferInfo* bufferInfos;
		VkDescriptorImageInfo* imageInfos;
		VkBufferView* texelBufferViews;
	};

	VkDescriptorType type;
	uint32_t binding;
	uint32_t arrayElem;

};

struct VKHdescriptorSets
{
	//intermediate:
	//---------------
	uint32_t count;
	DynamicArray<VKHdescriptorInfo> descriptors; //type - VKHdescriptorInfo

	//generated:
	//---------------
	vkh_bool_t generated;

	VkDescriptorPool pool;
	VkDescriptorSet* sets;
}; 


struct DrawState
{
	VKHinstance* instance;

	//drawing objects:
	VkFormat depthFormat;
	VkImage finalDepthImage;
	VkImageView finalDepthView;
	VkDeviceMemory finalDepthMemory;

	VkRenderPass finalRenderPass;

	uint32 framebufferCount;
	VkFramebuffer* framebuffers;

	VkCommandPool commandPool;
	VkCommandBuffer commandBuffers[FRAMES_IN_FLIGHT];

	VkSemaphore imageAvailableSemaphores[FRAMES_IN_FLIGHT];
	VkSemaphore renderFinishedSemaphores[FRAMES_IN_FLIGHT];
	VkFence inFlightFences[FRAMES_IN_FLIGHT];

	VkBuffer cameraBuffers[FRAMES_IN_FLIGHT];
	VkDeviceMemory cameraBuffersMemory[FRAMES_IN_FLIGHT];
	VkBuffer cameraStagingBuffer;
	VkDeviceMemory cameraStagingBufferMemory;

	//quad vertex buffers:
	VkBuffer quadVertexBuffer;
	VkDeviceMemory quadVertexBufferMemory;
	VkBuffer quadIndexBuffer;
	VkDeviceMemory quadIndexBufferMemory;

	//grid pipeline objects:
	VKHgraphicsPipeline* gridPipeline;
	VKHdescriptorSets* gridDescriptorSets;

	//particle pipeline objects:
	VKHgraphicsPipeline* particlePipeline;
	VKHdescriptorSets* particleDescriptorSets;

	VkDeviceSize particleBufferSize;
	VkBuffer particleBuffer;
	VkDeviceMemory particleBufferMemory;
};
