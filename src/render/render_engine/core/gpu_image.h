#pragma once

#include "core_common.h"
#include "gpu_resource.h"
#include <string>

NS_BEGIN

class GpuImage : public GpuResource {
public:
    GpuImage(const Device& device);
    ~GpuImage();

    void SetImageFormat(VkFormat format) { _format = format; _isDirty = true; }
    void SetImageAspect(VkImageAspectFlags aspectFlags) { _aspectFlags = aspectFlags; _isDirty = true; };
    void SetImageSize(uint32_t width, uint32_t height) { _texWidth = width; _texHeight = height; _isDirty = true; };

    VkImage GetImage() { return _textureImage; } 
    VkImageView GetImageView() { return _textureImageView; }
    VkSampler GetSampler() { return _textureSampler; }

    void UploadWithFile(const std::string& path);

    void Create();

    void Destroy() override;

protected:
    void updateBuffer(VkBuffer stagingBuffer, size_t bufferSize) override;
    void createTextureSampler();
    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

protected:
    uint32_t _texWidth = 0;
    uint32_t _texHeight = 0;

    VkImage _textureImage = VK_NULL_HANDLE;
    VkDeviceMemory _textureImageMemory = VK_NULL_HANDLE;
    VkImageView _textureImageView = VK_NULL_HANDLE;
    VkSampler _textureSampler = VK_NULL_HANDLE;
    VkFormat _format = VK_FORMAT_R8G8B8A8_SRGB;
    VkImageAspectFlags _aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;
};

NS_END