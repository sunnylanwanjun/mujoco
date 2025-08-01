#pragma once

#include "gpu_resource.h"

NS_BEGIN

class GpuBuffer : public GpuResource {
public:
    GpuBuffer(const Device& deivce);
    ~GpuBuffer();

    VkBuffer GetBuffer() { return _buffer; }
    void Destroy() override;

    void WriteWithPersistentMap(void* data, size_t bytesCount);
    void Create(size_t bufferSize);

    size_t GetBufferSize() const { return _bufferSize; }

protected:
    void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize) override;

private:
    size_t _bufferSize = 0;
    VkBuffer _buffer = VK_NULL_HANDLE;
    VkDeviceMemory _bufferMemory = VK_NULL_HANDLE;

    void* mapAddr = nullptr;
};

NS_END