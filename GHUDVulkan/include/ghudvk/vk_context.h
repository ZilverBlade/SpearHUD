#pragma once

#include <ghudcpp/ghud.h>
#include <vulkan/vulkan.h>

namespace GHUD {
	struct VkContextCreateInfo {
		VkDevice device;
		VkRenderPass renderPass;
		
	};

	class VkContext : public Context {
	public:
		VkContext(VkContextCreateInfo createInfo);
		virtual void Render(DrawList* drawList) override;
		ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
	private:
		VkDescriptorPool m_DescriptorPool;
		VkDevice device;
	};
}
