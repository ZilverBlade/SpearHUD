#pragma once

#include <ghudvk/core.h>
#include <ghudvk/api/vk_definitions.h>
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

	class ResourceDestructionObject : public virtual NonCopyableClass {
	public:
		ResourceDestructionObject(void* context, const void* object, void (*destroyFunc)(void* ctx, const void* userData))
			: ctx(context), ptr(object), dtor(destroyFunc) {}
		~ResourceDestructionObject() {
			dtor(ctx, ptr);
		}
	private:
		void* ctx;
		const void* ptr;
		void (*dtor)(void*, const void*);
	};

	struct VulkanFrameInfo {
		uint32 mFrameIndex;
		VkCommandBuffer mCommandBuffer;
	};

	using VulkanFrameInfoStruct = void;

	class VulkanTextShaderBufferObject : public TextShaderBuffer {
	public:
		GHUDVK_API ~VulkanTextShaderBufferObject();
	private:
		Buffer* buffer;
	};

	class VulkanContext : public Context {
	public:
		GHUDVK_API VulkanContext(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API ~VulkanContext();
		GHUDVK_API virtual void Pick() override;
		GHUDVK_API virtual void Render(const VulkanFrameInfoStruct* frameInfoStruct) override;
		GHUDVK_API ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
		GHUDVK_API void FreeTexture(const ResourceObject resource);

		// Creates uniform buffer for text data, or returns an existing buffer if already exists
		GHUDVK_API virtual BufferID AllocateTextBuffer(size_t id, const std::string& characters) override;

		// Marks text buffer free for deletion
		GHUDVK_API virtual void DeallocateTextBuffer(size_t id);

		// Destroys left over objects, must be called AFTER Render();
		GHUDVK_API virtual void Cleanup();
		GHUDVK_API void CreateResources(VkCommandBuffer singleTimeCommandBuffer);
		GHUDVK_API void FreeDescriptor(VkDescriptorSet descriptor);
	private:
		GHUDVK_API void CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo);
		GHUDVK_API void LoadFontMSDF(VkCommandBuffer singleTimeCommandBuffer);
		GHUDVK_API void CreateMSDFLUT();
		GHUDVK_API void CreateBuffers();

		std::vector<Buffer*> mGlobalUBO;
		std::vector<Buffer*> mIDSSBO;
		std::vector<VkDescriptorSet> mBufferDescriptorSets;

		std::vector<size_t> requestedTextObjects;
		std::vector<std::vector<std::shared_ptr<ResourceDestructionObject>>> safeDestructionQueue;

		VkDescriptorPool mDescriptorPool;
		VkDescriptorSetLayout mMSDFTextBufferDescriptorSetLayout;
		VkDescriptorSetLayout mTextureDescriptorSetLayout;
		VkDescriptorSetLayout mBufferDescriptorSetLayout;
		VkPipelineLayout mPipelineLayout;
		VkPipeline mGraphicsPipeline;
		VkPhysicalDevice mPhysicalDevice;
		VkDevice mDevice;

		VkShaderModule mVshModule;
		VkShaderModule mFshModule;

		Buffer* mMSDFLUTBuffer;
		Buffer* mFontBuffer;
		VkImage mFontImage;
		VkImageView mFontImageView;
		VkSampler mFontSampler;
		VkDeviceMemory mFontImageMemory;

		VkDescriptorSet mFontTexture;
		VkDescriptorSet mDummyTextBuffer;

		uint32 mSwapChainImageCount;

		uint32 mLastImageIndex = 0;
		
		friend class VulkanHUDInstance;
	};
}