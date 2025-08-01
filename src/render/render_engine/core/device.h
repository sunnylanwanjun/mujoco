#pragma once

#include "core_common.h"
#include "vulkan/vulkan_core.h"
#include <vector>
#include <optional>
#include "render_common.h"

NS_BEGIN

class Device {
public:
    Device();
    ~Device();

    void Init(void* win) { window = win; };
    void Destroy();
    void Create();

    VkDevice GetDevice() const { return device; }
    VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
    VkQueue GetGraphicsQueue() const { return graphicsQueue; }
    VkQueue GetPresentQueue() const { return presentQueue; }
    VkCommandPool GetCommandPool() const { return commandPool; }
    VkSurfaceKHR GetSurface() const { return surface; }
    void* GetWindow() const { return window; }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const;
    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const;
    VkFormat findDepthFormat() const;
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) const;
private:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void pickPhysicalDevice();
    void createLogicalDevice();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

    bool isDeviceSuitable(VkPhysicalDevice device);
    bool checkDeviceExtensionSupport(VkPhysicalDevice device);
    std::vector<const char*> getRequiredExtensions();
    bool checkValidationLayerSupport();
    void createCommandPool();

private:
    void* window = nullptr;
    VkInstance instance; 
    VkDebugUtilsMessengerEXT debugMessenger = nullptr;
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device = VK_NULL_HANDLE;

    VkQueue graphicsQueue = VK_NULL_HANDLE;
    VkQueue presentQueue = VK_NULL_HANDLE;
    VkCommandPool commandPool = VK_NULL_HANDLE;
};

NS_END