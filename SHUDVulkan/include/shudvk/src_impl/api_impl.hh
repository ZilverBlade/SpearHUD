#pragma once

#include <shudvk/api/buffer.h>
#include <shudvk/api/vk_calls.h>
#include <iostream>
#include <cassert>

namespace SHUD {
    Buffer::Buffer(
       VkDevice device,
       VkPhysicalDevice physDevice,
       VkDeviceSize size,
       VkBufferUsageFlags usageFlags,
       VkMemoryPropertyFlags memoryPropertyFlags
    ) : mDevice{ device },
        mUsageFlags{ usageFlags },
        mMemoryPropertyFlags{ memoryPropertyFlags } {
        mBufferSize = size;
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usageFlags;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device, &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS) {
            std::cerr << "[SHUD Vulkan] Failed to create Vulkan Buffer!";
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device, mBuffer, &memRequirements);

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

        if (vkAllocateMemory(device, &allocInfo, nullptr, &mMemory) != VK_SUCCESS) {
            std::cerr << "[SHUD Vulkan] Failed to allocate Vulkan buffer memory!";
        }

        vkBindBufferMemory(device, mBuffer, mMemory, 0);
    }
    Buffer::~Buffer() {
        Unmap();
        vkDestroyBuffer(mDevice, mBuffer, nullptr);
        vkFreeMemory(mDevice, mMemory, nullptr);
    }
    VkResult Buffer::Map(VkDeviceSize size) {
        assert(mBuffer && mMemory && "Called map on buffer before create");
        return vkMapMemory(mDevice, mMemory, 0, size, 0, &mMapped);
    }
    void Buffer::Unmap()  {
        if (mMapped) {
            vkUnmapMemory(mDevice, mMemory);
            mMapped = nullptr;
        }
    }
    void Buffer::WriteToBuffer(const void* data) {
        assert(mMapped && "Cannot copy to unmapped buffer");
        memcpy(mMapped, data, mBufferSize); 
    }
    void Buffer::WriteToBuffer(const void* data, VkDeviceSize size, VkDeviceSize offset) {
        assert(mMapped && "Cannot copy to unmapped buffer");
        memcpy((char*)mMapped + offset, data, size);
    }
    VkResult Buffer::Flush(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = mMemory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkFlushMappedMemoryRanges(mDevice, 1, &mappedRange);
    }
    VkDescriptorBufferInfo Buffer::GetDescriptorInfo(VkDeviceSize size, VkDeviceSize offset) {
        return VkDescriptorBufferInfo{
            mBuffer,
            offset,
            size == 0 ? mBufferSize : size,
        };
    }
    VkResult Buffer::Invalidate(VkDeviceSize size, VkDeviceSize offset) {
        VkMappedMemoryRange mappedRange = {};
        mappedRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
        mappedRange.memory = mMemory;
        mappedRange.offset = offset;
        mappedRange.size = size;
        return vkInvalidateMappedMemoryRanges(mDevice, 1, &mappedRange);
    }
}
