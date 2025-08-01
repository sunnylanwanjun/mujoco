#pragma once

#include "core_common.h"
#include <vulkan/vulkan_core.h>
#include <vector>
#include "gpu_image.h"

NS_BEGIN

class Device;
class SwapChain;
class Renderer;

class FrameBuffer {
public:
    FrameBuffer(const Device& device, const SwapChain& swapChain, const Renderer& renderer);
    ~FrameBuffer();

    void Destroy();
    void Create();

    VkFramebuffer GetFrameBuffer(size_t index) const {
        if (index < swapChainFramebuffers.size()) {
            return swapChainFramebuffers[index];
        }
        return VK_NULL_HANDLE;
    }

private:
    void CreateDepthResources();
    void CreateFramebuffers();

private:
    const Device& _device;
    const SwapChain& _swapChain;
    const Renderer& _renderer;

    GpuImage _depthTexture{_device};
    std::vector<VkFramebuffer> swapChainFramebuffers;
};

NS_END