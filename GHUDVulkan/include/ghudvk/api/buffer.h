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

        VkResult map(VkDeviceSize size = VK_WHOLE_SIZE);
        void unmap();

        void writeToBuffer(void* data);
        VkResult flush(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkDescriptorBufferInfo descriptorInfo(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);
        VkResult invalidate(VkDeviceSize size = VK_WHOLE_SIZE, VkDeviceSize offset = 0);

        inline VkBuffer GetBuffer() const { return m_Buffer; }
        inline void* GetMappedMemory() const { return m_Mapped; }
        inline VkBufferUsageFlags GetUsageFlags() const { return m_UsageFlags; }
        inline VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_MemoryPropertyFlags; }
        inline VkDeviceSize GetBufferSize() const { return m_BufferSize; }

    private:
        VkDevice m_Device;
        void* m_Mapped = nullptr;
        VkBuffer m_Buffer = nullptr;
        VkDeviceMemory m_Memory = nullptr;

        VkDeviceSize m_BufferSize;
        VkBufferUsageFlags m_UsageFlags;
        VkMemoryPropertyFlags m_MemoryPropertyFlags;
    };
}
