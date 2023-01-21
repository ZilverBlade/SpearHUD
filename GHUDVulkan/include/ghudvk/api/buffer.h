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

        VkBuffer GetBuffer() const { return m_Buffer; }
        void* GetMappedMemory() const { return m_Mapped; }
        VkBufferUsageFlags GetUsageFlags() const { return m_UsageFlags; }
        VkMemoryPropertyFlags GetMemoryPropertyFlags() const { return m_MemoryPropertyFlags; }
        VkDeviceSize GetBufferSize() const { return m_BufferSize; }

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
