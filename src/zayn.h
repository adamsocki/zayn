
#include <vector>
#include "commonlib.h"

#include "math/math.h"

#include "renderer/mesh.h"
#include "renderer/render.h"

#include "memory.h"

#include "dynamic_array.h"

#include "camera.h"



#include "input.h"



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


    InputManager inputManager;
    InputDevice* keyboard;
    InputDevice* mouse;

    MemoryArena permanentMemArena;
    MemoryArena frameMemArena;

    ShaderCollection shaderCollection;
    MeshCollection meshCollection;


    VKRenderData MyVKRenderData;

    VkInstance vkInstance;
    VkDebugUtilsMessengerEXT vkDebugMessenger;
    VkPhysicalDevice vkPhysicalDevice = VK_NULL_HANDLE;
    VkDevice vkDevice;
    VkPhysicalDeviceFeatures vkDeviceFeatures{};
    VkQueue vkGraphicsQueue;
    VkSurfaceKHR vkSurface;
    VkQueue vkPresentQueue;
    VkSwapchainKHR vkSwapChain;
    std::vector<VkImage> vkSwapChainImages;
    VkFormat vkSwapChainImageFormat;
    VkExtent2D vkSwapChainExtent;
    std::vector<VkImageView> vkSwapChainImageViews;
    VkPipelineLayout vkPipelineLayout;
    VkRenderPass vkRenderPass;
    VkPipeline vkGraphicsPipeline;
    std::vector<VkFramebuffer> vkSwapChainFramebuffers;
    VkCommandPool vkCommandPool;
    VkCommandBuffer vkCommandBuffer;
    VkSemaphore vkImageAvailableSemaphore;
    VkSemaphore vkRenderFinishedSemaphore;
    VkFence vkInFlightFence;


    VkCommandBuffer* vkCommandBuffers;
    std::vector<VkCommandBuffer> vkCommandBuffers2;

    std::vector<VkSemaphore> vkImageAvailableSemaphores;
    std::vector<VkSemaphore> vkRenderFinishedSemaphores;
    std::vector<VkFence> vkInFlightFences;
    uint32_t vkCurrentFrame = 0;

    bool vkFramebufferResized = false;
};


struct ZaynPlatform
{
    ZaynMemory zaynMemory;
    bool running;
};



ZaynMemory *Zayn = NULL;

InputManager *Input = NULL;
