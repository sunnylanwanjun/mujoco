#pragma once

#include "core_common.h"
#include <string>
#include <vulkan/vulkan_core.h>
#include <vector>

NS_BEGIN

class Device;

class Shader {
public:
    Shader(const Device& device);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(const Shader&&) = delete;

    void Destroy();
    void Create(const std::string& vs, const std::string& fs);

    VkShaderModule GetVS() { return vertShaderModule; }
    VkShaderModule GetFS() { return fragShaderModule; }
    std::vector<VkPipelineShaderStageCreateInfo> GetStageCreateInfo() { return createInfo; }

private:
    VkShaderModule createShaderModule(const std::vector<char>& code);

private:
    const Device& _device;

    VkShaderModule vertShaderModule = VK_NULL_HANDLE;
    VkShaderModule fragShaderModule = VK_NULL_HANDLE;

    std::vector<VkPipelineShaderStageCreateInfo> createInfo;
};

NS_END
