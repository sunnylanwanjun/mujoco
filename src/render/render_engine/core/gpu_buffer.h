#pragma once

#include "gpu_resource.h"

NS_BEGIN

class GpuBuffer : public GpuResource {
public:
    GpuBuffer();
    ~GpuBuffer();

    VkBuffer getBuffer() { return _buffer; }

protected:
    void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags property) override;

private:
    size_t _bufferSize = 0;
    VkBuffer _buffer;
    VkDeviceMemory _bufferMemory;
};

NS_END