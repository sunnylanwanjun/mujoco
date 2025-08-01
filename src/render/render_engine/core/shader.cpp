#include "shader.h"
#include <vector>
#include "file_utils.h"
#include <stdexcept>
#include "device.h"

NS_BEGIN

Shader::Shader(const Device& device):_device(device) {
    
}

Shader::~Shader() {
    Destroy();
}

void Shader::Destroy() {
    if (fragShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(_device.GetDevice(), fragShaderModule, nullptr);
        fragShaderModule = VK_NULL_HANDLE;
    }
    
    if (vertShaderModule != VK_NULL_HANDLE) {
        vkDestroyShaderModule(_device.GetDevice(), vertShaderModule, nullptr);
        vertShaderModule = VK_NULL_HANDLE;
    }
}

void Shader::Create(const std::string& vs, const std::string& fs) {
    Destroy();

    auto vertShaderCode = ReadFile(vs);
    auto fragShaderCode = ReadFile(fs);

    vertShaderModule = createShaderModule(vertShaderCode);
    fragShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    createInfo = {vertShaderStageInfo, fragShaderStageInfo};
}

VkShaderModule Shader::createShaderModule(const std::vector<char>& code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_device.GetDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }

    return shaderModule;
}

NS_END