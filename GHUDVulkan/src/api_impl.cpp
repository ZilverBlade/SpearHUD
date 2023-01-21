#pragma once

#include <ghudvk/api/buffer.h>
#include <iostream>
#include <cassert>

namespace GHUD {
    Buffer::Buffer(
       VkDevice device,
       VkPhysicalDevice physDevice,
       VkDeviceSize size,
       VkBufferUsageFlags usageFlags,
       VkMemoryPropertyFlags memoryPropertyFlags
    ) : m_Device{ device },
        m_UsageFlags{ usageFlags },
        m_MemoryPropertyFlags{ memoryPropertyFlags } {
        m_BufferSize = size;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
            std::cerr << "[GHUD Vulkan] Failed to create Vulkan Buffer!";
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;

        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(physDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((memRequirements.memoryTypeBits & (1 << i)) &&
                (memProperties.memoryTypes[i].propertyFlags & memoryPropertyFlags) == memoryPropertyFlags) {
                allocInfo.memoryTypeIndex = i;
                break;
            }
        }

        if (vkAllocateMemory(device, &allocInfo, nullptr, &m_Memory) != VK_SUCCESS) {
            std::cerr << "[GHUD Vulkan] Failed to allocate Vulkan buffer memory!";
        }

        vkBindBufferMemory(device, m_Buffer, m_Memory, 0);
    }
    Buffer::~Buffer() {
        Unmap();
        vkDestroyBuffer(m_Device, m_Buffer, nullptr);
        vkFreeMemory(m_Device, m_Memory, nullptr);
    }
    VkResult Buffer::Map(VkDeviceSize size) {
        assert(m_Buffer && m_Memory && "Called map on buffer before create");
        return vkMapMemory(m_Device, m_Memory, 0, size, 0, &m_Mapped);
    }
    void Buffer::Unmap()  {
        if (m_Mapped) {
            vkUnmapMemory(m_Device, m_Memory);
            m_Mapped = nullptr;
        }
    }
    void Buffer::WriteToBuffer(void* data) {
        assert(m_Mapped && "Cannot copy to unmapped buffer");
        memcpy(m_Mapped, data, m_BufferSize); 
    }
    VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_Memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(m_Device, 1, &mappedRange);
    }
    VkDescriptorBufferInfo Buffer::GetDescriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            m_Buffer,
            offset,
            size,
        };
    }
    VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = m_Memory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(m_Device, 1, &mappedRange);
    }
}
