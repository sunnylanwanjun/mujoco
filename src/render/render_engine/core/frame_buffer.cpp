#include "frame_buffer.h"
#include <stdexcept>
#include <array>

NS_BEGIN

FrameBuffer::FrameBuffer(const Device& device, const SwapChain& swapChain): _device(device), _swapChain(swapChain), _depthTexture{device} {

}

FrameBuffer::~FrameBuffer() {
    Destroy();
}

void FrameBuffer::Destroy() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(_device.GetDevice(), framebuffer, nullptr);
    }
    swapChainFramebuffers.clear();
}

void FrameBuffer::Create(VkRenderPass renderPass) {
    Destroy();

    _renderPass = renderPass;

    createDepthResources();
    createFramebuffers();
}

void FrameBuffer::createDepthResources() {
    VkExtent2D swapChainExtent = _swapChain.GetExtent();

    _depthTexture.SetImageSize(swapChainExtent.width, swapChainExtent.height);
    VkFormat depthFormat = findDepthFormat();
    _depthTexture.SetImageFormat(depthFormat);
    _depthTexture.SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    _depthTexture.Create();        
}

void FrameBuffer::createFramebuffers() {
    auto& swapChainImageViews = _swapChain.GetSwapChainImageViews();
    swapChainFramebuffers.resize(swapChainImageViews.size());
    VkExtent2D swapChainExtent = _swapChain.GetExtent();

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            _depthTexture.GetImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(_device.GetDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

VkFormat FrameBuffer::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const {
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(_device.GetPhysicalDevice(), format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

VkFormat FrameBuffer::findDepthFormat() const {
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

NS_END