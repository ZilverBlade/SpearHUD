#pragma once

#include <vulkan/vulkan.h>

namespace GHUD {
    class Buffer {
    public:
        Buffer(
            VkDevice device,
            VkPhysicalDevice physDevice,
            VkDeviceSize size,
            VkBufferUsageFlags usageFlags,
            VkMemoryPropertyFlags memoryPropertyFlags
        );
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        VkResult Map(VkDeviceSize size = VK_WHOLE_SIZE);
        void Unmap();

        void WriteToBuffer(void* data);
        VkResult Flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo GetDescriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult Invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        VkBuffer GetBuffer() const { return mBuffer; }
        void* GetMappedMemory() const { return mMapped; }
        VkBufferUsageFlags GetUsageFlags() const { return mUsageFlags; }
        VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return mMemoryPropertyFlags; }
        VkDeviceSize GetBufferSize() const { return mBufferSize; }

    private:
        VkDevice mDevice;
        void* mMapped = nullptr;
        VkBuffer mBuffer = nullptr;
        VkDeviceMemory mMemory = nullptr;

        VkDeviceSize mBufferSize;
        VkBufferUsageFlags mUsageFlags;
        VkMemoryPropertyFlags mMemoryPropertyFlags;
    };
}
