#pragma once

#include "core_common.h"
#include "gpu_buffer.h"

NS_BEGIN

class GpuImage : public GpuBuffer {
public:
    GpuImage();
    ~GpuImage();

protected:
    void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize, VkBufferUsageFlags usage,  VkMemoryPropertyFlags property) override;
};

NS_END