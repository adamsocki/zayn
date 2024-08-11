
// void sierpinski(
//     std::vector<Vertex>& vertices,
//     int32_t depth,
//     vec2 left,
//     vec2 right,
//     vec2 top) {
//   if (depth <= 0) {
//     vertices.push_back({top});
//     vertices.push_back({right});
//     vertices.push_back({left});
//   } else {
//     vec2 leftTop = {0.5f * (left.x + top.x), 0.5f * (left.y + top.y)};
//     vec2 rightTop = {0.5f * (right.x + top.x), 0.5f * (right.y + top.y)};
//     vec2 leftRight = {0.5f * (left.x + right.x), 0.5f * (left.y + right.y)};
//     sierpinski(vertices, depth - 1, left, leftRight, leftTop);
//     sierpinski(vertices, depth - 1, leftRight, right, rightTop);
//     sierpinski(vertices, depth - 1, leftTop, rightTop, top);
//   }
// }

#include <cstring>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"


namespace std
{
    template <>
    struct hash<Vertex>
    {
        size_t operator()(Vertex const &vertex) const
        {
            size_t seed = 0;
            hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
}

namespace std {
    template<> struct hash<Vertex_> {
        size_t operator()(Vertex_ const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

Builder CreateCubeModelBuilder()
{
    Builder modelBuilder = {};
   
    modelBuilder.vertices =
    {
        {{-.5f,-.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f,-.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f,-.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f,-.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f,-.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f,-.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}}
    };

    modelBuilder.indices = {0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                          12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21};

    return modelBuilder;
}

std::vector<Vertex> CreateCubeModel2()
{
    return {
        {{-.5f,-.5f, -.5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f,-.5f, .5f}, {.9f, .9f, .9f}},
        {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

        // right face (yellow)
        {{.5f,-.5f, -.5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
        {{.5f,-.5f, .5f}, {.8f, .8f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

        // top face (orange, remember y axis points down)
        {{-.5f,-.5f, -.5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
        {{-.5f,-.5f, .5f}, {.9f, .6f, .1f}},
        {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

        // bottom face (red)
        {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
        {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

        // nose face (blue)
        {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
        {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

        // tail face (green)
        {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
        {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},

    };
}



uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, ZaynMemory* zaynMem)
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

void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, ZaynMemory *zaynMem)
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

void CreateBuffer(
    VkDeviceSize size,
    VkBufferUsageFlags usage,
    VkMemoryPropertyFlags properties,
    VkBuffer &buffer,
    VkDeviceMemory &bufferMemory,
    ZaynMemory* zaynMem)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(zaynMem->vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(zaynMem->vkDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, zaynMem);

    if (vkAllocateMemory(zaynMem->vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    vkBindBufferMemory(zaynMem->vkDevice, buffer, bufferMemory, 0);
}

// void CreateTextureImage(ZaynMemory* zaynMem)
// {
//     int texWidth, texHeight, texChannels;
//     stbi_uc* pixels = stbi_load("/Users/socki/dev/zayn/models/textures/texture_test.jpg", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
//     VkDeviceSize imageSize = texWidth * texHeight * 4;

//     if (!pixels) {
//         throw std::runtime_error("failed to load texture image!");
//     }
// }

void CreateVertexBuffers(const std::vector<Vertex>& vertices, Model* model, ZaynMemory* zaynMem)
{   // todo: update to match index buffer method below
    model->vertexCount = static_cast<uint32_t>(vertices.size());

    assert(model->vertexCount >= 3 && "Vertex Count must be atleast 3.");
    
    VkDeviceSize bufferSize = sizeof(vertices[0]) * model->vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory,
        zaynMem);

    

    void *data;
    vkMapMemory(zaynMem->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(zaynMem->vkDevice, stagingBufferMemory);

    CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        model->vertexBuffer,
        model->vertexBufferMemory,
        zaynMem);

    CopyBuffer(stagingBuffer, model->vertexBuffer, bufferSize, zaynMem);

    vkDestroyBuffer(zaynMem->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, stagingBufferMemory, nullptr);


}

void CreateIndexBuffer(const std::vector<uint32_t> &indices, Model* model, ZaynMemory* zaynMem)
{
    model->indexCount = static_cast<uint32_t>(indices.size());
    model->hasIndexBuffer = model->indexCount > 0;

    if (!model->hasIndexBuffer)
    {
        return;
    }

    VkDeviceSize bufferSize = sizeof(indices[0]) * model->indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory,
        zaynMem);

    void *data;
    vkMapMemory(zaynMem->vkDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(zaynMem->vkDevice, stagingBufferMemory);

    CreateBuffer(
        bufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        model->indexBuffer,
        model->indexBufferMemory,
        zaynMem);

    CopyBuffer(stagingBuffer, model->indexBuffer, bufferSize, zaynMem);

    vkDestroyBuffer(zaynMem->vkDevice, stagingBuffer, nullptr);
    vkFreeMemory(zaynMem->vkDevice, stagingBufferMemory, nullptr);
}

void CreateUniformBuffers(ZaynMemory* zaynMem)
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    zaynMem->vkUniformBuffers.resize(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkUniformBuffersMemory.resize(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);
    zaynMem->vkUniformBuffersMapped.resize(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < zaynMem->VK_MAX_FRAMES_IN_FLIGHT; i++) 
    {
        CreateBuffer(bufferSize, 
                    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
                    VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 
                    zaynMem->vkUniformBuffers[i], 
                    zaynMem->vkUniformBuffersMemory[i],
                    zaynMem);

        vkMapMemory(zaynMem->vkDevice, 
                    zaynMem->vkUniformBuffersMemory[i], 
                    0, 
                    bufferSize, 
                    0, 
                    &zaynMem->vkUniformBuffersMapped[i]);
    }
}

void DrawModel(VkCommandBuffer commandBuffer, Model* model)
{
    if (model->hasIndexBuffer) 
    {
        vkCmdDrawIndexed(commandBuffer, model->indexCount, 1, 0, 0, 0);
    }
    else
    {
        vkCmdDraw(commandBuffer, model->vertexCount, 1, 0, 0);
    }
}

void BindModel(VkCommandBuffer commandBuffer, Model* model)
{   
    VkBuffer buffers[] = {model->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    if (model->hasIndexBuffer) 
    {
        vkCmdBindIndexBuffer(commandBuffer, model->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}

void ModelInit(VkDevice *device, Builder modelBuilder, Model *model, ZaynMemory* zaynMem)
{
    model->vkDevice = device;
    model->vertexCount = static_cast<int32_t>(modelBuilder.vertices.size());;

    // CreateTextureImage(zaynMem);
    CreateVertexBuffers(modelBuilder.vertices, model, zaynMem);
    CreateIndexBuffer(modelBuilder.indices, model, zaynMem);
    CreateUniformBuffers(zaynMem);
   
    
}


// void EntityInit(VkDevice *device, const std::vector<Vertex>& vertices, Enitty *model, ZaynMemory* zaynMem)
// {
//     model->vkDevice = device;
//     // model->vertexCount = static_cast<int32_t>(vertices.size());;

//     CreateVertexBuffers(vertices, model, zaynMem);

// }

std::vector<VkVertexInputBindingDescription> getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);

    attributeDescriptions[2].binding = 0;
    attributeDescriptions[2].location = 2;
    attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[2].offset = offsetof(Vertex, normal);

    attributeDescriptions[3].binding = 0;
    attributeDescriptions[3].location = 3;
    attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[3].offset = offsetof(Vertex, uv);
    return attributeDescriptions;
}

void LoadModel(const std::string &filepath, Builder *modelBuilder, Model *model)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string error;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &error, filepath.c_str()))
    {
        throw std::runtime_error(warn + error);
    }

    modelBuilder->vertices.clear();
    modelBuilder->indices.clear();

    std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex = {};
            if (index.vertex_index >= 0)
            {
                vertex.position = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2],
                };

                 vertex.color = {
                    attrib.colors[3 * index.vertex_index + 0],
                    attrib.colors[3 * index.vertex_index + 1],
                    attrib.colors[3 * index.vertex_index + 2],
                };
            }

            if (index.normal_index >= 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2],
                };
            }

            if (index.texcoord_index >= 0)
            {
                vertex.uv = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            // modelBuilder->vertices.push_back(vertex);

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(modelBuilder->vertices.size());
                modelBuilder->vertices.push_back(vertex);
            }
            modelBuilder->indices.push_back(uniqueVertices[vertex]);
        }
        std::cout << "Unique vertices:  " << uniqueVertices.size() << std::endl;
    }
}

void CreateModelFromFile(VkDevice *device, const std::string &filepath, Model *model, ZaynMemory* zaynMem)
{
    Builder modelBuilder = {};
    
    
    LoadModel(filepath, &modelBuilder, model); 
    std::cout << "Vertex Size Count: " << modelBuilder.vertices.size() << std::endl;
    std::cout << "Index Size Count: " << modelBuilder.indices.size() << std::endl;
    ModelInit(&zaynMem->vkDevice, modelBuilder, model, zaynMem);

}

void CreateDescriptorPools(ZaynMemory *zaynMem)
{
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;
    poolInfo.maxSets = static_cast<uint32_t>(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);

    if (vkCreateDescriptorPool(zaynMem->vkDevice, &poolInfo, nullptr, &zaynMem->vkDescriptorPool) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void CreateDescriptorSets(ZaynMemory *zaynMem)
{
    std::vector<VkDescriptorSetLayout> layouts(zaynMem->VK_MAX_FRAMES_IN_FLIGHT, zaynMem->vkDescriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = zaynMem->vkDescriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);
    allocInfo.pSetLayouts = layouts.data();

    zaynMem->vkDescriptorSets.resize(zaynMem->VK_MAX_FRAMES_IN_FLIGHT);
    if (vkAllocateDescriptorSets(zaynMem->vkDevice, &allocInfo, zaynMem->vkDescriptorSets.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < zaynMem->VK_MAX_FRAMES_IN_FLIGHT; i++)
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

        vkUpdateDescriptorSets(zaynMem->vkDevice, 1, &descriptorWrite, 0, nullptr);
    }
}
