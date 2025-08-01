#pragma once

#include "core_common.h"
#include "render_node.h"

#include "core/geo_utils.h"
#include "core/gpu_buffer.h"
#include "core/gpu_image.h"
#include "core/shader.h"
#include <string>

NS_BEGIN

class MeshRenderNode : public RenderNode {
public:
    MeshRenderNode(const Device& device, const Renderer& renderer);
    ~MeshRenderNode();

    MeshRenderNode(const MeshRenderNode&) = delete;
    MeshRenderNode& operator=(const MeshRenderNode&) = delete;
    MeshRenderNode(const MeshRenderNode&&) = delete;

    void Destroy() override;
    void Create() override;
    void Execute(VkCommandBuffer commandBuffer) override;

    void SetBaseTexture(const std::string& path);
    void SetUniformBufferSize(size_t bufferSize);
    void SetMesh(const Mesh& mesh);
    void SetShader(const std::string& vs, const std::string& fs);

    void UpdateUniformBuffer(void* data, size_t size);

private:
    void CreateDescriptorSetLayout();
    void CreateGraphicsPipeline();
    void CreateDescriptorPool();
    void CreateDescriptorSets();
    
private:
    VkDescriptorSetLayout _descriptorSetLayout{ VK_NULL_HANDLE };
    VkPipelineLayout _pipelineLayout{ VK_NULL_HANDLE };
    VkPipeline _graphicsPipeline{ VK_NULL_HANDLE };

    GpuImage _texture{ _device };
    GpuBuffer _vertexBuffer{ _device };
    GpuBuffer _indexBuffer{ _device };
    Shader _shader{ _device };
    std::vector<GpuBuffer> _uniformBuffers;

    VkDescriptorPool _descriptorPool{ VK_NULL_HANDLE };
    std::vector<VkDescriptorSet> _descriptorSets{ VK_NULL_HANDLE };

    size_t indicesCount = 0;
};

NS_END