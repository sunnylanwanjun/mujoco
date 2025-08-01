#include "gpu_buffer.h"
#include <memory>
#include "device.h"

NS_BEGIN

GpuBuffer::GpuBuffer(const Device& device): GpuResource(device) {
}

GpuBuffer::~GpuBuffer() {
    Destroy();
}

void GpuBuffer::Destroy() {
    if (_buffer != VK_NULL_HANDLE) {
        vkDestroyBuffer(_device.GetDevice(), _buffer, nullptr);
        _buffer = VK_NULL_HANDLE;
    }
    
    if (_bufferMemory != VK_NULL_HANDLE) {
        vkFreeMemory(_device.GetDevice(), _bufferMemory, nullptr);
        _bufferMemory = VK_NULL_HANDLE;
    }

    _isDirty = true;
}

void GpuBuffer::updateBuffer(VkBuffer stagingBuffer, size_t bufferSize) {
    if (bufferSize > _bufferSize) {
       _isDirty = true;
    }

    if (_isDirty) {
        Destroy();
        _bufferSize = bufferSize;
        createBuffer(bufferSize, _usage, _property, _buffer, _bufferMemory);
        _isDirty = false;
    }

    copyBuffer(stagingBuffer, _buffer, bufferSize);
}

void GpuBuffer::Create(size_t bufferSize) {
    if (bufferSize > _bufferSize) {
       _isDirty = true;
    }

    if (_isDirty) {
        Destroy();
        _bufferSize = bufferSize;
        createBuffer(bufferSize, _usage, _property, _buffer, _bufferMemory);
        _isDirty = false;
    }
}

void GpuBuffer::WriteWithPersistentMap(void* data, size_t bytesCount) {
    if (mapAddr == nullptr) {
        vkMapMemory(_device.GetDevice(), _bufferMemory, 0, _bufferSize, 0, &mapAddr);
    }

    memcpy(mapAddr, data, bytesCount);
}

NS_END