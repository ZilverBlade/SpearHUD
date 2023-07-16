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
		~VulkanTextShaderBufferObject();
	private:
		Buffer* buffer;
	};

	class VulkanContext : public Context {
	public:
		VulkanContext(const VulkanContextCreateInfo& createInfo);
		~VulkanContext();
		virtual void Pick() override;
		virtual void Render(const VulkanFrameInfoStruct* frameInfoStruct) override;
		ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
		void FreeTexture(const ResourceObject resource);

		// Creates uniform buffer for text data, or returns an existing buffer if already exists
		virtual BufferID AllocateTextBuffer(size_t id, const std::string& characters) override;
		// Marks text buffer free for deletion
		virtual void DeallocateTextBuffer(size_t id);
	
		// Destroys left over objects, must be called AFTER Render();
		virtual void Cleanup();

		void CreateResources(VkCommandBuffer singleTimeCommandBuffer);
		void FreeDescriptor(VkDescriptorSet descriptor);


	private:
		void CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo);
		void LoadFontMSDF(VkCommandBuffer singleTimeCommandBuffer);
		void CreateMSDFLUT();
		void CreateBuffers();

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