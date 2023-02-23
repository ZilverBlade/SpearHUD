#pragma once

#include <ghudvk/core.h>
#include <ghudvk/api/buffer.h>

namespace GHUD {
	struct VulkanContextCreateInfo {
		VkDevice m_Device;
		VkPhysicalDevice m_PhysicalDevice;
		VkRenderPass m_RenderPass;
		uint32 m_SubPass;
		VkFormat m_FrameBufferFormat;
		uint32 m_SwapChainImageCount;
		VkSampleCountFlagBits m_MSAASamples;

		void* m_VshCodeOverride = nullptr;
		size_t m_VshCodeOverrideSize;
		void* m_FshCodeOverride = nullptr;
		size_t m_FshCodeOverrideSize;
	};

	struct VulkanFrameInfo {
		uint32 m_FrameIndex;
		VkCommandBuffer m_CommandBuffer;
	};

	using VulkanFrameInfoStruct = void;

	class VulkanContext : public Context {
	public:
		GHUDVK_API VulkanContext(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API ~VulkanContext();
		GHUDVK_API virtual void Render(const VulkanFrameInfoStruct* frameInfoStruct) override;
		GHUDVK_API ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
		GHUDVK_API void CreateResources(VkCommandBuffer singleTimeCommandBuffer);
	private:
		GHUDVK_API void CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API void CreateBlankTexture(VkCommandBuffer singleTimeCommandBuffer);

		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSetLayout m_TextureDescriptorSetLayout;
		VkDescriptorSetLayout m_BufferDescriptorSetLayout;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;

		VkShaderModule m_VshModule;
		VkShaderModule m_FshModule;

		Buffer* m_EBuffer;
		VkImage m_EImage;
		VkImageView m_EImageView;
		VkSampler m_ESampler;
		VkDeviceMemory m_EImageMemory;

		VkDescriptorSet m_ETexture;

		uint32 m_SwapChainImageCount;
		std::vector<Buffer*> m_GlobalUBO;
		std::vector<Buffer*> m_IDSSBO;
		std::vector<VkDescriptorSet> m_BufferDescriptorSets;
	};
}
