
struct Vertex
{
    vec3 position;
    vec3 color;


    VkVertexInputBindingDescription* bindingDescriptions;
    VkVertexInputAttributeDescription* attributeDescriptions;
};


struct Model
{
    VkDevice* vkDevice;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    int32 vertexCount;
};


