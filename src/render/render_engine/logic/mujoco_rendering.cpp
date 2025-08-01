#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>
#include <unordered_map>

#include "application.h"

#include "core/geo_utils.h"
#include "core/gpu_buffer.h"
#include "core/gpu_image.h"
#include "core/swap_chain.h"
#include "core/device.h"
#include "core/renderer.h"
#include "core/frame_buffer.h"
#include "core/render_common.h"
#include "core/file_utils.h"
#include "core/mesh_render_node.h"
#include <memory>

using namespace RenderEngine;

const std::string MODEL_PATH = WORKING_DIR"models/viking_room.obj";
const std::string TEXTURE_PATH = WORKING_DIR"textures/viking_room.png";
constexpr int MAX_FRAMES_IN_FLIGHT = 2;

struct UniformBufferObject {
    alignas(16) glm::mat4 model;
    alignas(16) glm::mat4 view;
    alignas(16) glm::mat4 proj;
};

class MujocoRendering : public Application {
protected:
    Device _device;
    SwapChain _swapChain{ _device };
    Renderer _renderer{ _device, _swapChain, MAX_FRAMES_IN_FLIGHT };
    FrameBuffer _frameBuffer{ _device, _swapChain, _renderer};
    std::shared_ptr<MeshRenderNode> meshRenderNode = nullptr;

    void _Init() override {
        _device.Init(window);
        _device.Create();
        _swapChain.Create();
        _renderer.Create();
        _frameBuffer.Create();

        Mesh mesh;
        mesh.loadModel(MODEL_PATH);
        meshRenderNode = std::make_shared<MeshRenderNode>(_device, _renderer);
        meshRenderNode->SetUniformBufferSize(sizeof(UniformBufferObject));
        meshRenderNode->SetMesh(mesh);
        meshRenderNode->SetBaseTexture(TEXTURE_PATH);
        meshRenderNode->SetShader(WORKING_DIR"shaders/27_shader_depth.vert.spv", WORKING_DIR"shaders/27_shader_depth.frag.spv");
        meshRenderNode->Create();
        _renderer.AddRenderNode(std::static_pointer_cast<RenderNode>(meshRenderNode));
    }

    void _UnInit() override {}

    void RecreateSwapChain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize((GLFWwindow*)window, &width, &height);
            glfwWaitEvents();
        }

        vkDeviceWaitIdle(_device.GetDevice());
        _swapChain.Create();
        _frameBuffer.Create();
    }

    void UpdateUniformBuffer() {
        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        VkExtent2D viewportSize = _swapChain.GetViewportSize();

        UniformBufferObject ubo{};
        ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.proj = glm::perspective(glm::radians(45.0f), viewportSize.width / (float) viewportSize.height, 0.1f, 10.0f);
        ubo.proj[1][1] *= -1;

        if (meshRenderNode) {
            meshRenderNode->UpdateUniformBuffer(&ubo, sizeof(ubo));
        }
    }

    void _Update() {
        UpdateUniformBuffer();
        auto res = _renderer.Execute(_frameBuffer);
        if (!res) {
            RecreateSwapChain();
        }
    }
};

APPLICATION_REG(MujocoRendering)