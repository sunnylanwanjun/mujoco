#pragma once

#include "core_common.h"
#include "vulkan/vulkan_core.h"
#include "gpu_image.h"
#include <vector>

NS_BEGIN

class Device;

class SwapChain {
public:
    SwapChain(const Device& device);
    ~SwapChain();

    void Destroy();
    void Create();

    VkFormat GetFormat() const { return swapChainImageFormat; }

    VkExtent2D GetViewportSize() const { return swapChainExtent; }
    VkSwapchainKHR GetSwapChain() const { return swapChain; }

    const std::vector<VkImageView>& GetSwapChainImageViews() const {
        return swapChainImageViews;
    }

private:
    void createSwapChain();
    void createImageViews();

    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

private:
    const Device& _device;

    VkSwapchainKHR swapChain = VK_NULL_HANDLE;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat = VK_FORMAT_UNDEFINED;
    VkExtent2D swapChainExtent { 0, 0 };
    std::vector<VkImageView> swapChainImageViews;
};

NS_END