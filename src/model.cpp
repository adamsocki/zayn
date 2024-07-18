
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

    CreateVertexBuffers(modelBuilder.vertices, model, zaynMem);
    CreateIndexBuffer(modelBuilder.indices, model, zaynMem);

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
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset = offsetof(Vertex, position);

    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
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

    // std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
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

                auto colorIndex = 3 * index.vertex_index + 2;
                if (colorIndex < attrib.colors.size())
                {
                    vertex.color = {
                        attrib.colors[colorIndex - 2],
                        attrib.colors[colorIndex - 1],
                        attrib.colors[colorIndex - 0],
                    };
                }
                else
                {
                    vertex.color = {1.f, 1.f, 1.f}; // set default color
                }
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

            modelBuilder->vertices.push_back(vertex);

            // if (uniqueVertices.count(vertex) == 0)
            // {
            //     uniqueVertices[vertex] = static_cast<uint32_t>(modelBuilder->vertices.size());
            //     modelBuilder->vertices.push_back(vertex);
            // }
            // modelBuilder->indices.push_back(uniqueVertices[vertex]);
        }
    }
}

void CreateModelFromFile(VkDevice *device, const std::string &filepath, Model *model, ZaynMemory* zaynMem)
{
    Builder modelBuilder = {};
    
    
    LoadModel(filepath, &modelBuilder, model); 
    ModelInit(&zaynMem->vkDevice, modelBuilder, model, zaynMem);

}