#include <ghudvk/vk_context.h>

namespace GHUD {
	ResourceObject VkContext::CreateTexture(const VkDescriptorImageInfo& imageInfo) {
		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

		vkCreateDescriptorSetLayout(device, &layoutCreateInfo, nullptr, nullptr /* todo */);

		VkDescriptorSet set = {};
		VkDescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = m_DescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = nullptr; // todo
		vkAllocateDescriptorSets(device, &allocateInfo, &set);

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.dstBinding = 0;
		write.dstSet = set;
		write.pImageInfo = &imageInfo;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vkUpdateDescriptorSets(device, 1, &write, 0, nullptr);
		return *reinterpret_cast<ResourceObject*>(&set);
	}
}