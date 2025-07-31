#pragma once

#include "core_common.h"
#include "device.h"
#include <string>
#include <vulkan/vulkan_core.h>

NS_BEGIN

class Shader {
public:
    Shader(const Device& device, const std::string& vs, const std::string& fs);
    ~Shader();

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
