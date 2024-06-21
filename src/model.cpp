
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
{
    model->vertexCount = static_cast<uint32_t>(vertices.size());

    assert(model->vertexCount >= 3 && "Vertex Count must be atleast 3.");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * model->vertexCount;

    CreateBuffer(bufferSize,  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
      model->vertexBuffer, model->vertexBufferMemory, zaynMem);

    void* data;
    vkMapMemory(zaynMem->vkDevice, model->vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(zaynMem->vkDevice, model->vertexBufferMemory);

}

void DrawModel(VkCommandBuffer commandBuffer, Model* model)
{
    vkCmdDraw(commandBuffer, model->vertexCount, 1, 0, 0);
}

void BindModel(VkCommandBuffer commandBuffer, Model* model)
{   
    VkBuffer buffers[] = {model->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
}

void ModelInit(VkDevice *device, const std::vector<Vertex>& vertices, Model *model, ZaynMemory* zaynMem)
{
    model->vkDevice = device;
    // model->vertexCount = static_cast<int32_t>(vertices.size());;

    CreateVertexBuffers(vertices, model, zaynMem);

}

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
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(1);
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
    attributeDescriptions[0].offset = 0;
    return attributeDescriptions;
}