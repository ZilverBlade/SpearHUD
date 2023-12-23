#pragma once

#include <shudvk/core.h>
#include <shudvk/api/vk_definitions.h>
#include <shudvk/api/buffer.h>

namespace SHUD {
	struct VulkanContextCreateInfo {
		VkDevice mDevice = VK_NULL_HANDLE;
		VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
		VkRenderPass mRenderPass = VK_NULL_HANDLE;
		uint32 mSubPass = 0;
		VkFormat mFramebufferFormat = VK_FORMAT_UNDEFINED;
		uint32 mSwapChainImageCount = 0;
		VkSampleCountFlagBits mMSAASamples = VK_SAMPLE_COUNT_1_BIT;

		VkCullModeFlags mDrawCullMode = VK_CULL_MODE_NONE;

		void* mVshCodeOverride = nullptr;
		size_t mVshCodeOverrideSize;
		void* mFshCodeOverride = nullptr;
		size_t mFshCodeOverrideSize;
	};

	struct VulkanContextStatistics {
		uint32_t mTotalAllocatedDescriptors;
		uint32_t mRecycledDescriptors;
		uint32_t mAllocatedTextures;
		uint32_t mAllocatedTextBuffers;
		uint32_t mDrawCalls;
		uint32_t mVertices;
		float mCpuRenderTime;
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

	class VulkanContext : public Context {
	public:
		VulkanContext(const VulkanContextCreateInfo& createInfo);
		~VulkanContext();
		virtual void Pick() override;
		virtual void Render(const VulkanFrameInfoStruct* frameInfoStruct) override;
		ResourceObject CreateTexture(const VkDescriptorImageInfo& imageInfo);
		void FreeTexture(const ResourceObject resource);

		// Creates uniform buffer for text data, or returns an existing buffer if already exists
		virtual BufferID AllocateTextBuffer(size_t id, const std::string& characters, const TextFormatting& formatting) override;
		// Marks text buffer free for deletion
		virtual void DeallocateTextBuffer(size_t id);
	
		// Destroys left over objects, must be called AFTER Render();
		virtual void Cleanup();

		void CreateResources(VkCommandBuffer singleTimeCommandBuffer);
		void FreeDescriptor(VkDescriptorSet descriptor);

		void DrawDebugInfo();

		const VulkanContextStatistics& GetContextStatistics() const {
			return mStatistics;
		}
	private:
		void CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo);
		void LoadFontMSDF(VkCommandBuffer singleTimeCommandBuffer);
		void CreateMSDFLUT();
		void CreateBuffers();

		void RecycleTextDescriptor(VkDescriptorSet descriptor);

		std::vector<Buffer*> mGlobalUBO;
		std::vector<Buffer*> mIDSSBO;
		std::vector<VkDescriptorSet> mBufferDescriptorSets;

		std::vector<size_t> mRequestedTextObjects;
		std::vector<std::vector<std::shared_ptr<ResourceDestructionObject>>> mSafeDestructionQueue;
		std::vector<VkDescriptorSet> mOrphanedTextDescriptorSets; // recycle old descriptor sets

		uint32_t mMaxAllowedTextDescriptors;

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

		VulkanContextStatistics mStatistics;

		uint32 mSwapChainImageCount;
		uint32 mLastImageIndex = 0;

		friend class ResourceDestructionObject;
	};
}