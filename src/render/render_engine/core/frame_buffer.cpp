#include "frame_buffer.h"
#include <stdexcept>
#include <array>
#include "device.h"
#include "swap_chain.h"
#include "renderer.h"

NS_BEGIN

FrameBuffer::FrameBuffer(const Device& device, const SwapChain& swapChain, const Renderer& renderer): _device(device), _swapChain(swapChain), _renderer(renderer) {

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

void FrameBuffer::Create() {
    Destroy();

    CreateDepthResources();
    CreateFramebuffers();
}

void FrameBuffer::CreateDepthResources() {
    VkExtent2D swapChainExtent = _swapChain.GetViewportSize();

    _depthTexture.SetImageSize(swapChainExtent.width, swapChainExtent.height);
    VkFormat depthFormat = _device.findDepthFormat();
    _depthTexture.SetImageFormat(depthFormat);
    _depthTexture.SetUsage(VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT);
    _depthTexture.Create();        
}

void FrameBuffer::CreateFramebuffers() {
    auto& swapChainImageViews = _swapChain.GetSwapChainImageViews();
    swapChainFramebuffers.resize(swapChainImageViews.size());
    VkExtent2D swapChainExtent = _swapChain.GetViewportSize();

    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            _depthTexture.GetImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderer.GetRenderPass();
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

NS_END