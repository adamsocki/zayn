struct Vertex_
{
    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription()
    {
        VkVertexInputBindingDescription bindingDescription{};
        bindingDescription.binding = 0;
        bindingDescription.stride = sizeof(Vertex_);
        bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescription;
    }

    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions()
    {
        std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex_, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex_, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex_, texCoord);

        return attributeDescriptions;
    }

    bool operator==(const Vertex_& other) const {
    return pos == other.pos && color == other.color && texCoord == other.texCoord;
}
};
#include <unordered_map>

#include <memory>
#include <vector>

struct UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 proj;
};

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;

    VkVertexInputBindingDescription *bindingDescriptions;
    VkVertexInputAttributeDescription *attributeDescriptions;

    bool operator==(const Vertex &other) const
    {
        return position == other.position &&
               color == other.color &&
               normal == other.normal &&
               uv == other.uv;
    }
};

struct Index
{
};

struct Builder
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};

struct Model
{
    VkDevice *vkDevice;
    bool hasIndexBuffer = false;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    int32 vertexCount;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    int32 indexCount;
};
