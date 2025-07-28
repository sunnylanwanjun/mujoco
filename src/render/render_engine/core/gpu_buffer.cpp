#include "gpu_buffer.h"

NS_BEGIN

GpuBuffer::GpuBuffer() {
}

GpuBuffer::~GpuBuffer() {
    vkDestroyBuffer(_device, _buffer, nullptr);
    vkFreeMemory(_device, _bufferMemory, nullptr);
}

void GpuBuffer::updateBuffer(VkBuffer stagingBuffer, size_t bufferSize, VkBufferUsageFlags usage, VkMemoryPropertyFlags property) {
     if (bufferSize > _bufferSize) {
        if (_bufferSize > 0) {
            vkDestroyBuffer(_device, _buffer, nullptr);
            vkFreeMemory(_device, _bufferMemory, nullptr);
            _bufferSize = 0;
        }
        _bufferSize = bufferSize;
        createBuffer(bufferSize, usage, property, _buffer, _bufferMemory);
    }
    copyBuffer(stagingBuffer, _buffer, bufferSize);
}

NS_END