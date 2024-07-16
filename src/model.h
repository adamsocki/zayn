
struct Vertex
{
    vec3 position;
    vec3 color;


    VkVertexInputBindingDescription* bindingDescriptions;
    VkVertexInputAttributeDescription* attributeDescriptions;
};

struct Builder
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};


struct Model
{
    VkDevice* vkDevice;
    bool hasIndexBuffer = false;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    int32 vertexCount;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    int32 indexCount;
};


