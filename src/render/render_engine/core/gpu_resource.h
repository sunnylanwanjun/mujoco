#pragma once

#include "core_common.h"
#include "vulkan/vulkan.h"

NS_BEGIN

class Device;

class GpuResource {
public:
    GpuResource(const Device& device);
    virtual ~GpuResource();
    
    void Upload(const void* srcData, size_t bytesCount);
    void SetUsage(VkBufferUsageFlags usage) { _usage = usage; _isDirty = true; }
    void SetProperty(VkMemoryPropertyFlags property) { _property = property; _isDirty = true; }
    virtual void Destroy() = 0;

protected:
    virtual void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize) = 0;
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

    VkCommandBuffer beginSingleTimeCommands();

    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

protected:
    bool _isDirty = true;
    const Device& _device;

    VkBufferUsageFlags _usage = 0;
    VkMemoryPropertyFlags _property = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
};

NS_END