
#include <vector>
#include "commonlib.h"

#include "math/math.h"

#include "model.h"

#include "renderer/mesh.h"
#include "renderer/render.h"

#include "memory.h"

#include "dynamic_array.h"

#include "camera.h"



#include "input.h"


#include "casette/casette.h"



struct MeshCollection
{
    Mesh triangleMesh;
    Mesh quadMesh;
};

struct ShaderCollection
{
    Shader shader_test_01;

};



struct ZaynMemory
{
    GLFWwindow* window;
    vec2 windowSize;

    Model model1;

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

    VkBuffer vkVertexBuffer;
    VkDeviceMemory vkVertexBufferMemory;
    VkBuffer vkIndexBuffer;
    VkDeviceMemory vkIndexBufferMemory;

    VkDescriptorSetLayout vkDescriptorSetLayout;

    std::vector<VkBuffer> vkUniformBuffers;
    std::vector<VkDeviceMemory> vkUniformBuffersMemory;
    std::vector<void*> vkUniformBuffersMapped;

    VkDescriptorPool vkDescriptorPool;
    std::vector<VkDescriptorSet> vkDescriptorSets;

    vkMesh triangleMesh;
    // AllocateTriangle(&triangleMesh, vulkanMem);

    vkMesh quadMesh;
    // AllocateQuad(&quadMesh, vulkanMem);


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
