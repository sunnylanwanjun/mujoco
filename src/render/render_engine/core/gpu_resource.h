#pragma once

#include "core_common.h"
#include "vulkan/vulkan.h"

NS_BEGIN

class GpuResource {
public:
    GpuResource();
    virtual ~GpuResource();
    void Init(VkDevice device, 
        VkCommandPool commandPool, 
        VkQueue graphicsQueue,
        VkPhysicalDevice physicalDevice);
    
    void Upload(void* srcData, size_t bytesCount, VkBufferUsageFlags usage, VkMemoryPropertyFlags property);

protected:
    virtual void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags property) = 0;

    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

protected:
    VkDevice _device;
    VkCommandPool _commandPool;
    VkQueue _graphicsQueue;
    VkPhysicalDevice _physicalDevice = VK_NULL_HANDLE;
};

NS_END