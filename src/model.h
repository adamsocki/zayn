#include <unordered_map>


#include <memory>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec3 normal;
    glm::vec2 uv;


    VkVertexInputBindingDescription* bindingDescriptions;
    VkVertexInputAttributeDescription* attributeDescriptions;

    bool operator==(const Vertex& other) const {
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
    VkDevice* vkDevice;
    bool hasIndexBuffer = false;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    int32 vertexCount;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    int32 indexCount;
};


