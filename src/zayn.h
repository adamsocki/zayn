
#include <vector>
#include "commonlib.h"

#include "math/math.h"

#include "buffer.h"
#include "model.h"


#include "memory.h"

#include "dynamic_array.h"

#include "renderer/mesh.h"
#include "renderer/render.h"
#include "renderer/render_vulkan.h"


#include "entity.h"


#include "camera.h"



#include "input.h"


#include "casette/casette.h"



enum ZaynState
{
    ZAYN_BASE_STATE,
    ZAYN_CARTRIDGE_STATE
};

struct MeshCollection
{
    Mesh triangleMesh;
    Mesh quadMesh;
};

struct ShaderCollection
{
    Shader shader_test_01;

};

struct Pipelines
{
    VkPipeline instancedRocks = VK_NULL_HANDLE;
    VkPipeline planet = VK_NULL_HANDLE;
    VkPipeline starfield = VK_NULL_HANDLE;
};

struct ZaynMemory
{
    GLFWwindow* window;
    ZaynState zaynState = ZAYN_BASE_STATE;

    vec2 windowSize;

    Model model1;
    Model model2;
    Model casette_model;

    Monkey monkey_1;
    EntityHandle monkeyHandle1;
    EntityHandle casette_handle_1;

    InputManager inputManager;
    InputDevice* keyboard;
    InputDevice* mouse;

    MemoryArena permanentMemArena;
    MemoryArena frameMemArena;

    ShaderCollection shaderCollection;
    MeshCollection meshCollection;


    VKRenderData MyVKRenderData;

    PipelineConfigInfo MyPipelineConfigInfo = {};

    VkPipeline vkGraphicsPipeline;
    VkPipeline vkGraphicsPipeline2;
    
    VkShaderModule vkVertShaderModule;
    VkShaderModule vkFragShaderModule;

    VkInstance vkInstance;
    VkDebugUtilsMessengerEXT vkDebugMessenger;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice vkDevice;
    VkPhysicalDeviceFeatures vkDeviceFeatures{};
    VkQueue vkGraphicsQueue;
    VkSurfaceKHR vkSurface;
    VkQueue vkPresentQueue;
    VkSwapchainKHR vkSwapChain;

    VkSwapchainKHR vkOldSwapChain;

    std::vector<VkImage> vkDepthImages;
    std::vector<VkDeviceMemory> vkDepthImageMemorys;
    std::vector<VkImageView> vkDepthImageViews;
    std::vector<VkImage> vkSwapChainImages;

    VkFormat vkSwapChainImageFormat;
    VkExtent2D vkSwapChainExtent;
    std::vector<VkImageView> vkSwapChainImageViews;
    VkPipelineLayout vkPipelineLayout;
    VkRenderPass vkRenderPass;
    // VkPipeline vkGraphicsPipeline;
    std::vector<VkFramebuffer> vkSwapChainFramebuffers;
    VkCommandPool vkCommandPool;
    VkCommandBuffer vkCommandBuffer;
    VkSemaphore vkImageAvailableSemaphore;
    VkSemaphore vkRenderFinishedSemaphore;
    VkFence vkInFlightFence;

    // VkCommandBuffer* vkCommandBuffers;
    std::vector<VkCommandBuffer> vkCommandBuffers;

    std::vector<VkSemaphore> vkImageAvailableSemaphores;
    std::vector<VkSemaphore> vkRenderFinishedSemaphores;
    std::vector<VkFence> vkInFlightFences;
    std::vector<VkFence> vkImagesInFlight;

    uint32_t vkCurrentFrame = 0;
    

    bool vkFramebufferResized = false;

    bool vkHasIndexBuffer = false;
    VkBuffer vkVertexBuffer;
    VkDeviceMemory vkVertexBufferMemory;
    VkBuffer vkIndexBuffer;
    VkDeviceMemory vkIndexBufferMemory;
    uint32 vkIndexCount;
    

    VkBuffer vkInstanceBuffer;
    VkDeviceMemory vkInstanceBufferMemory;

    VkDescriptorSetLayout vkDescriptorSetLayout;

    std::vector<VkBuffer> vkUniformBuffers;
    std::vector<VkDeviceMemory> vkUniformBuffersMemory;
    std::vector<void*> vkUniformBuffersMapped;

    std::vector<VkBuffer> vkUniformBuffers2;
    std::vector<VkDeviceMemory> vkUniformBuffers2Memory;
    std::vector<void*> vkUniformBuffers2Mapped;

    std::vector<VkBuffer> vkUniformBuffersDynamic;
    std::vector<VkDeviceMemory> vkUniformBuffersDynamicMemory;
    std::vector<void*> vkUniformBuffersDynamicMapped;

    VkDescriptorPool vkDescriptorPool;
    std::vector<VkDescriptorSet> vkDescriptorSets;


    uint32_t vkMipLevels;
    VkImage vkTextureImage;
    VkDeviceMemory vkTextureImageMemory;
    VkImageView vkTextureImageView;
    VkSampler vkTextureSampler;

    VkImage vkDepthImage;
    VkDeviceMemory vkDepthImageMemory;
    VkImageView vkDepthImageView;

    std::vector<Vertex_> vkVertices;
    std::vector<uint32_t> vkIndices;

    
    int32 VK_MAX_FRAMES_IN_FLIGHT = 2;

    // VKHgraphicsPipeline vkGraphicsPipeline_NEW;
    // VKHinstance vkInstance_NEW;
    DrawState* drawState;
    GameCamera gameCamera;
    real32 accumTime = 0.0f;
	uint32 accumFrames = 0;


    vkMesh triangleMesh;
    // AllocateTriangle(&triangleMesh, vulkanMem);

    vkMesh quadMesh;
    // AllocateQuad(&quadMesh, vulkanMem);



    uint32 vkCurrentImageIndex;
    bool vkIsFrameStarted = false;
    VkCommandBuffer vkNextCommandBuffer;


    Pipelines vkPipelines;


    Camera camera;

    uint64 systemTime;
    real32 time;
    real32 deltaTime;
    real32 startTime;
        
    uint32 frame;
    real32 fps;


    void *myCasette;
};


struct ZaynManagers
{

};


struct ZaynPlatform
{
    ZaynMemory zaynMemory;
    bool running;
};



ZaynMemory *Zayn = NULL;

InputManager *Input = NULL;

InputDevice *Keyboard = NULL;
