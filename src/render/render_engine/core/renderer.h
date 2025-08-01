#pragma once

#include "core_common.h"
#include "device.h"
#include <memory>

NS_BEGIN

class Device;
class SwapChain;
class FrameBuffer;
class RenderNode;

class Renderer {
public:
    Renderer(const Device& device, const SwapChain& swapChain, const size_t maxFrameInFlight);
    ~Renderer();

    void Destroy();
    void Create();
    bool Execute(const FrameBuffer& frameBuffer);

    size_t GetMaxFrameInFlight() const { return _maxFrameInFlight; }
    VkRenderPass GetRenderPass() const { return _renderPass; }
    size_t GetCurrentFrame() const { return _currentFrame; }

    VkExtent2D GetViewportSize() const;

    void AddRenderNode(std::shared_ptr<RenderNode> renderNode) { renderNodes.push_back(renderNode); }

private:
    void CreateRenderPass();
    void CreateCommandBuffers();
    void RecordCommandBuffer(const FrameBuffer& frameBuffer, VkCommandBuffer commandBuffer, size_t imageIndex);
    void CreateSyncObjects();

private:
    const Device& _device;
    const SwapChain& _swapChain;

    size_t _maxFrameInFlight = 1;

    VkRenderPass _renderPass;

    std::vector<VkCommandBuffer> _commandBuffers;

    std::vector<VkSemaphore> _imageAvailableSemaphores;
    std::vector<VkSemaphore> _renderFinishedSemaphores;
    std::vector<VkFence> _inFlightFences;
    size_t _currentFrame = 0;

    std::vector<std::shared_ptr<RenderNode>> renderNodes;
};

NS_END