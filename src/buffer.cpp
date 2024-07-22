

// void CreateBuffer2(
//     VkDeviceSize size,
//     VkBufferUsageFlags usage,
//     VkMemoryPropertyFlags properties,
//     VkBuffer &buffer,
//     VkDeviceMemory &bufferMemory,
//     ZaynMemory* zaynMem)
// {
//     VkBufferCreateInfo bufferInfo{};
//     bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
//     bufferInfo.size = size;
//     bufferInfo.usage = usage;
//     bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

//     if (vkCreateBuffer(zaynMem->vkDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
//     {
//         throw std::runtime_error("failed to create vertex buffer!");
//     }

//     VkMemoryRequirements memRequirements;
//     vkGetBufferMemoryRequirements(zaynMem->vkDevice, buffer, &memRequirements);

//     VkMemoryAllocateInfo allocInfo{};
//     allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
//     allocInfo.allocationSize = memRequirements.size;
//     allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties, zaynMem);

//     if (vkAllocateMemory(zaynMem->vkDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
//     {
//         throw std::runtime_error("failed to allocate vertex buffer memory!");
//     }

//     vkBindBufferMemory(zaynMem->vkDevice, buffer, bufferMemory, 0);
// }

// void InitBuffer(VkDeviceSize bufferInstanceSize, 
//                 uint32 instanceCount, 
//                 VkBufferUsageFlags usageFlags,
//                 VkMemoryPropertyFlags memoryPropertyFlags,
//                 VkDeviceSize minOffsetAlignment = 1, 
//                 VkBuffer* buffer)
// {
//     // CreateBuffer2(bufferInstanceSize, usageFlags, memoryPropertyFlags, buffer, buffer->memory, zaynMem);
// }

// void MapBuffer(MyVkBuffer* buffer, ZaynMemory* zaynMem)
// {
//     //  assert(buffer && memory && "Called map on buffer before create");
//     //  return vkMapMemory(zaynMem->d, memory, offset, size, 0, &mapped);
// }

// void UnMapBuffer()
// {

// }

// void WriteToBuffer()
// {

// }