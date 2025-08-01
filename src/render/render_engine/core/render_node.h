#pragma once

#include "core_common.h"
#include <vulkan/vulkan_core.h>

NS_BEGIN

class Device;
class Renderer;

class RenderNode {
public:
    RenderNode(const Device& device, const Renderer& renderer);
    virtual ~RenderNode();

    RenderNode(const RenderNode&) = delete;
    RenderNode& operator=(const RenderNode&) = delete;
    RenderNode(const RenderNode&&) = delete;

    virtual void Destroy() = 0;
    virtual void Create() = 0;

    virtual void Execute(VkCommandBuffer commandBuffer) = 0;

protected:
    const Renderer& _renderer;
    const Device& _device;
};

NS_END