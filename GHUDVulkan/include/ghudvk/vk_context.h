#pragma once

#include <ghudvk/core.h>
#include <ghudvk/api/buffer.h>

namespace GHUD {
	struct VulkanContextCreateInfo {
		VkDevice mDevice;
		VkPhysicalDevice mPhysicalDevice;
		VkRenderPass mRenderPass;
		uint32 mSubPass;
		VkFormat mFrameBufferFormat;
		uint32 mSwapChainImageCount;
		VkSampleCountFlagBits mMSAASamples;

		void* mVshCodeOverride = nullptr;
		size_t mVshCodeOverrideSize;
		void* mFshCodeOverride = nullptr;
		size_t mFshCodeOverrideSize;
	};

	struct VulkanFrameInfo {
		uint32 mFrameIndex;
		VkCommandBuffer mCommandBuffer;
	};

	using VulkanFrameInfoStruct = void;

	class VulkanContext : public Context {
	public:
		GHUDVK_API VulkanContext(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API ~VulkanContext();
		GHUDVK_API virtual void Pick() override;
		GHUDVK_API virtual void Render(const VulkanFrameInfoStruct* frameInfoStruct) override;
		GHUDVK_API ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
		GHUDVK_API void CreateResources(VkCommandBuffer singleTimeCommandBuffer);
	private:
		GHUDVK_API void CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API void CreateBlankTexture(VkCommandBuffer singleTimeCommandBuffer);

		VkDescriptorPool mDescriptorPool;
		VkDescriptorSetLayout mTextureDescriptorSetLayout;
		VkDescriptorSetLayout mBufferDescriptorSetLayout;
		VkPipelineLayout mPipelineLayout;
		VkPipeline mGraphicsPipeline;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		VkShaderModule mVshModule;
		VkShaderModule mFshModule;

		Buffer* mEBuffer;
		VkImage mEImage;
		VkImageView mEImageView;
		VkSampler mESampler;
		VkDeviceMemory mEImageMemory;

		VkDescriptorSet mETexture;

		uint32 mSwapChainImageCount;

		uint32 mLastImageIndex = 0;
		std::vector<Buffer*> mGlobalUBO;
		std::vector<Buffer*> mIDSSBO;
		std::vector<VkDescriptorSet> mBufferDescriptorSets;
	};
}
