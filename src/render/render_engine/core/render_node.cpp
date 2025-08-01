#include "render_node.h"
#include "device.h"
#include "renderer.h"

NS_BEGIN

RenderNode::RenderNode(const Device& device, const Renderer& renderer): _device(device), _renderer(renderer) {
}

RenderNode::~RenderNode() {}

NS_END