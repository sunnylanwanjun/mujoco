#pragma once

#include "core_common.h"
#include "device.h"
#include "swap_chain.h"

NS_BEGIN

class FrameBuffer {
public:
    FrameBuffer(const Device& device, const SwapChain& swapChain);
    ~FrameBuffer();

    void Destroy();
    void Create(VkRenderPass renderPass);

    VkFramebuffer GetFrameBuffer(size_t index) {
        if (index < swapChainFramebuffers.size()) {
            return swapChainFramebuffers[index];
        }
        return VK_NULL_HANDLE;
    }

    VkFormat findDepthFormat() const;
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;

private:
    void createDepthResources();
    void createFramebuffers();

private:
    const Device& _device;
    const SwapChain& _swapChain;

    GpuImage _depthTexture;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    VkRenderPass _renderPass = VK_NULL_HANDLE;
};

NS_END