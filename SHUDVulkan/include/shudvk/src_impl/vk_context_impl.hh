#include <vulkan/vk_enum_string_helper.h>

#include <shudvk/vk_context.h>
#include <shudvk/vk_shader_binary.h>
#include <shudvk/api/vk_calls.h>

#include <shudcpp/draw/draw_data.h>
#include <shudcpp/resources/text_object.h>
#include <shudcpp/default_msdf_atlas.h>

#include <chrono>

namespace SHUD {
	struct ShaderUBO {
		fvec2 mCursorCoord{};
		fvec2 mResolution;
		fvec2 mInvResolution;
		float mAspectRatio;
		float mInvAspectRatio;

		float mDisplayGamma;
		float mInvDisplayGamma;
	};

#define SHUDVK_MAX_DESCRIPTOR_SETS 2048u + 1024u + 1u

	VulkanContext::VulkanContext(const VulkanContextCreateInfo& createInfo) : mDevice(createInfo.mDevice), mPhysicalDevice(createInfo.mPhysicalDevice), mSwapChainImageCount(createInfo.mSwapChainImageCount) {
		mVtblAssert = this;
		mStatistics = {};

		mMaxAllowedTextDescriptors = 2000;
		VkDescriptorPoolSize poolSizes[] = {
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2048 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 },
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = SHUDVK_MAX_DESCRIPTOR_SETS;
		poolInfo.poolSizeCount = 3u;
		poolInfo.pPoolSizes = poolSizes;
		vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool);

		VkDescriptorSetLayoutBinding textureBinding{};
		textureBinding.binding = 0;
		textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		textureBinding.descriptorCount = 1;
		textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		VkDescriptorSetLayoutCreateInfo textureLayoutCreateInfo{};
		textureLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		textureLayoutCreateInfo.bindingCount = 1;
		textureLayoutCreateInfo.pBindings = &textureBinding;
		vkCreateDescriptorSetLayout(mDevice, &textureLayoutCreateInfo, nullptr, &mTextureDescriptorSetLayout);


		VkDescriptorSetLayoutBinding bufferCharDataBindings[2];
		bufferCharDataBindings[0].binding = 0;
		bufferCharDataBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT;
		bufferCharDataBindings[0].descriptorCount = 1;
		bufferCharDataBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bufferCharDataBindings[0].pImmutableSamplers = nullptr;

		bufferCharDataBindings[1].binding = 1;
		bufferCharDataBindings[1].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		bufferCharDataBindings[1].descriptorCount = 1;
		bufferCharDataBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		bufferCharDataBindings[1].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo msdfTextLayoutCreateInfo{};
		msdfTextLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		msdfTextLayoutCreateInfo.bindingCount = 2;
		msdfTextLayoutCreateInfo.pBindings = bufferCharDataBindings;
		vkCreateDescriptorSetLayout(mDevice, &msdfTextLayoutCreateInfo, nullptr, &mMSDFTextBufferDescriptorSetLayout);

		VkDescriptorSetLayoutBinding bufferBindings[3]{};
		bufferBindings[0].binding = 0;
		bufferBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		bufferBindings[0].descriptorCount = 1;
		bufferBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					  
		bufferBindings[1].binding = 1;
		bufferBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bufferBindings[1].descriptorCount = 1;
		bufferBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		bufferBindings[2].binding = 2;
		bufferBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bufferBindings[2].descriptorCount = 1;
		bufferBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		VkDescriptorSetLayoutCreateInfo bufferLayoutCreateInfo{};
		bufferLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		bufferLayoutCreateInfo.bindingCount = 3;
		bufferLayoutCreateInfo.pBindings = bufferBindings;
		vkCreateDescriptorSetLayout(mDevice, &bufferLayoutCreateInfo, nullptr, &mBufferDescriptorSetLayout);

		static_assert(sizeof(DrawData) <= 128, "DrawData exceeds size of recommended vulkan spec push constant size of 128 bytes");

		VkPushConstantRange pushRange{};
		pushRange.offset = 0;
		pushRange.size = sizeof(DrawData);
		pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		VkDescriptorSetLayout setLayouts[3]{
			mBufferDescriptorSetLayout,
			mTextureDescriptorSetLayout,
			mMSDFTextBufferDescriptorSetLayout
		};

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 3;
		pipelineLayoutInfo.pSetLayouts = setLayouts; // global UBO, texture, and text
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushRange; // push
		vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr, &mPipelineLayout);

		mSafeDestructionQueue.resize(createInfo.mSwapChainImageCount);

		CreateMSDFLUT();
		CreateBuffers();
		CreateGraphicsPipeline(createInfo);
	}

	VulkanContext::~VulkanContext() {
		vkDeviceWaitIdle(mDevice);
		delete mFontBuffer;
		delete mMSDFLUTBuffer;

		for (uint32 i = 0; i < mSwapChainImageCount; i++) {
			delete mGlobalUBO[i];
			delete mIDSSBO[i];
		}

		for (auto& [id, buffer] : mTextBuffers) {
			DeallocateTextBuffer(id);
		}
		for (auto& q : mSafeDestructionQueue) {
			q.clear();
		}

		vkDestroySampler(mDevice, mFontSampler, nullptr);
		vkDestroyImageView(mDevice, mFontImageView, nullptr);
		vkDestroyImage(mDevice, mFontImage, nullptr);
		vkFreeMemory(mDevice, mFontImageMemory, nullptr);

		vkDestroyPipeline(mDevice, mGraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);

		vkDestroyDescriptorSetLayout(mDevice, mMSDFTextBufferDescriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(mDevice, mTextureDescriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(mDevice, mBufferDescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);
	}

	void VulkanContext::Pick() {
		size_t id = *reinterpret_cast<size_t*>(mIDSSBO[mLastImageIndex]->GetMappedMemory());
		mCtxData.selectedObject = id;
	//	std::cout << id << " is reported id\n";
	}

	void VulkanContext::CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo) {
		VkShaderModuleCreateInfo vshModuleCreateInfo{};
		vshModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		if (createInfo.mVshCodeOverride) {
			vshModuleCreateInfo.codeSize = createInfo.mVshCodeOverrideSize;
			vshModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.mVshCodeOverride);
		} else {
			vshModuleCreateInfo.codeSize = sizeof(vk_vsh_bin);
			vshModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vk_vsh_bin);
		}

		VkShaderModuleCreateInfo fshModuleCreateInfo{};
		fshModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

		if (createInfo.mFshCodeOverride) {
			fshModuleCreateInfo.codeSize = createInfo.mFshCodeOverrideSize;
			fshModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(createInfo.mFshCodeOverride);
		} else {
			fshModuleCreateInfo.codeSize = sizeof(vk_fsh_bin);
			fshModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vk_fsh_bin);
		}

		vkCreateShaderModule(mDevice, &vshModuleCreateInfo, nullptr, &mVshModule);
		vkCreateShaderModule(mDevice, &fshModuleCreateInfo, nullptr, &mFshModule);

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = mVshModule;
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = mFshModule;
		shaderStages[1].pName = "main";
		shaderStages[1].flags = 0;
		shaderStages[1].pNext = nullptr;
		shaderStages[1].pSpecializationInfo = nullptr;

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr;
		vertexInputInfo.pVertexBindingDescriptions = nullptr;

		VkPipelineViewportStateCreateInfo viewportInfo{};
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo{};
		VkPipelineRasterizationStateCreateInfo rasterizationInfo{};
		VkPipelineMultisampleStateCreateInfo multisampleInfo{};
		std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments{};
		VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo{};
		std::vector<VkDynamicState> dynamicStateEnables{};
		VkPipelineDynamicStateCreateInfo dynamicStateInfo{};

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = shaderStages.size();
		pipelineInfo.pStages = shaderStages.data();
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;
		pipelineInfo.pViewportState = &viewportInfo;
		pipelineInfo.pRasterizationState = &rasterizationInfo;
		pipelineInfo.pMultisampleState = &multisampleInfo;
		pipelineInfo.pColorBlendState = &colorBlendInfo;
		pipelineInfo.pDepthStencilState = &depthStencilInfo;
		pipelineInfo.pDynamicState = &dynamicStateInfo;

		pipelineInfo.layout = mPipelineLayout;
		pipelineInfo.renderPass = createInfo.mRenderPass;
		pipelineInfo.subpass = createInfo.mSubPass;

		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.viewportCount = 1;
		viewportInfo.pViewports = nullptr;
		viewportInfo.scissorCount = 1;
		viewportInfo.pScissors = nullptr;

		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.depthClampEnable = VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizationInfo.lineWidth = 1.0f;
		rasterizationInfo.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizationInfo.depthBiasEnable = VK_FALSE;
		rasterizationInfo.depthBiasConstantFactor = 0.0f;
		rasterizationInfo.depthBiasClamp = 0.0f;
		rasterizationInfo.depthBiasSlopeFactor = 0.0f;

		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.sampleShadingEnable = createInfo.mMSAASamples == VK_SAMPLE_COUNT_1_BIT ? VK_FALSE : VK_TRUE;
		multisampleInfo.rasterizationSamples = createInfo.mMSAASamples;
		multisampleInfo.minSampleShading = 1.0f;
		multisampleInfo.pSampleMask = nullptr;
		multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleInfo.alphaToOneEnable = VK_FALSE;

		colorBlendAttachments.resize(1);
		colorBlendAttachments[0].colorWriteMask =
			VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
			VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachments[0].blendEnable = VK_TRUE;
		colorBlendAttachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachments[0].colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachments[0].alphaBlendOp = VK_BLEND_OP_ADD;

		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.logicOpEnable = VK_FALSE;
		colorBlendInfo.logicOp = VK_LOGIC_OP_COPY; 
		colorBlendInfo.attachmentCount = 1;
		colorBlendInfo.pAttachments = colorBlendAttachments.data();
		colorBlendInfo.blendConstants[0] = 0.0f; 
		colorBlendInfo.blendConstants[1] = 0.0f; 
		colorBlendInfo.blendConstants[2] = 0.0f; 
		colorBlendInfo.blendConstants[3] = 0.0f; 

		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.depthTestEnable = VK_FALSE; // no depth test
		depthStencilInfo.depthWriteEnable = VK_FALSE;
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_NEVER;
		depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
		depthStencilInfo.minDepthBounds = 0.0f;
		depthStencilInfo.maxDepthBounds = 1.0f;
		depthStencilInfo.stencilTestEnable = VK_FALSE;
		depthStencilInfo.front = {}; 
		depthStencilInfo.back = {};  

		dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
		dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicStateInfo.pDynamicStates = dynamicStateEnables.data();
		dynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStateEnables.size());
		dynamicStateInfo.flags = 0;

		pipelineInfo.basePipelineIndex = -1;
		pipelineInfo.basePipelineHandle = nullptr;

		VkResult pipelineResult = vkCreateGraphicsPipelines(mDevice, nullptr, 1, &pipelineInfo, nullptr, &mGraphicsPipeline);
		assert(pipelineResult == VK_SUCCESS && "Failed to create graphics pipeline!");

		vkDestroyShaderModule(mDevice, mVshModule, nullptr);
		vkDestroyShaderModule(mDevice, mFshModule, nullptr);
	}

	void VulkanContext::Render(const VulkanFrameInfoStruct* frameInfoStruct) {
		auto cpuRenderBeginTiming = std::chrono::high_resolution_clock::now();
		mStatistics.mDrawCalls = 0;
		mStatistics.mVertices = 0;

		const VulkanFrameInfo& frameInfo = *reinterpret_cast<const VulkanFrameInfo*>(frameInfoStruct);
		mLastImageIndex = frameInfo.mFrameIndex;
		
		ShaderUBO ubo{};
		ubo.mAspectRatio = mCtxData.mAspectRatio;
		ubo.mInvAspectRatio = mCtxData.mInvAspectRatio;
		ubo.mCursorCoord = mIO.mCursorPosition;
		ubo.mDisplayGamma = mCtxData.mDisplayGamma;
		ubo.mInvDisplayGamma = mCtxData.mInvDisplayGamma;
		ubo.mResolution = mCtxData.mResolution;
		ubo.mInvResolution = mCtxData.mInvResolution;

		mGlobalUBO[frameInfo.mFrameIndex]->WriteToBuffer(&ubo);
		mGlobalUBO[frameInfo.mFrameIndex]->Flush();

		// reset ID for next frame to avoid "ghost selection"
		size_t clearID = 0x00;
		mIDSSBO[frameInfo.mFrameIndex]->WriteToBuffer(&clearID);
		mIDSSBO[frameInfo.mFrameIndex]->Flush();

		vkCmdBindPipeline(frameInfo.mCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

		VkDescriptorSet sets[3]{
			mBufferDescriptorSets[frameInfo.mFrameIndex],
			mFontTexture, // bind font texture by default to avoid validation errors
			mDummyTextBuffer // random uniform buffer, we dont care which one, as long as it fills in the descriptor slot
		};

		vkCmdBindDescriptorSets(
			frameInfo.mCommandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			mPipelineLayout,
			0,
			3,
			sets,
			0,
			nullptr
		);

		for (const DrawInfo& draw : mDrawList->GetList()) {
			if (draw.mData.mFlags & SHUD_DRAW_DATA_FLAG_HAS_TEXTURE) {
				VkDescriptorSet textureDescriptor = *reinterpret_cast<const VkDescriptorSet*>(&draw.mTextureID);
				VkDescriptorSet bufferDescriptor = mDummyTextBuffer;
				if (draw.mData.mFlags & SHUD_DRAW_DATA_FLAG_IS_MSDF_TEXT) {
					bufferDescriptor = *reinterpret_cast<const VkDescriptorSet*>(&draw.mBufferID);
				}
				const VkDescriptorSet descriptors[]{ textureDescriptor, bufferDescriptor };
				vkCmdBindDescriptorSets(
					frameInfo.mCommandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					mPipelineLayout,
					1,
					2,
					descriptors,
					0,
					nullptr
				);
			}
			vkCmdPushConstants(frameInfo.mCommandBuffer, mPipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(DrawData), &draw.mData);
			vkCmdDraw(frameInfo.mCommandBuffer, 4, draw.mInstanceCount, 0, 0);
			mStatistics.mDrawCalls++;
			mStatistics.mVertices += 4 * draw.mInstanceCount;
		}

		auto cpuRenderEndTiming = std::chrono::high_resolution_clock::now();
		mStatistics.mCpuRenderTime = std::chrono::duration<float, std::chrono::seconds::period>(cpuRenderEndTiming - cpuRenderBeginTiming).count();
	}

	void VulkanContext::CreateBuffers() {
		mGlobalUBO.resize(mSwapChainImageCount);
		mIDSSBO.resize(mSwapChainImageCount);
		mBufferDescriptorSets.resize(mSwapChainImageCount);

		VkDescriptorBufferInfo msdfLut = mMSDFLUTBuffer->GetDescriptorInfo();
		for (uint32 i = 0; i < mSwapChainImageCount; i++) {
			mGlobalUBO[i] = new Buffer(mDevice, mPhysicalDevice, sizeof(ShaderUBO), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			mIDSSBO[i] = new Buffer(mDevice, mPhysicalDevice, sizeof(uint64), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
			mGlobalUBO[i]->Map();
			mIDSSBO[i]->Map();

			VkDescriptorSet set = {};
			VkDescriptorSetAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = mDescriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &mBufferDescriptorSetLayout;
			vkAllocateDescriptorSets(mDevice, &allocateInfo, &set);

			VkWriteDescriptorSet writes[3]{};

			VkDescriptorBufferInfo uboInfo = mGlobalUBO[i]->GetDescriptorInfo();
			writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[0].descriptorCount = 1;
			writes[0].dstBinding = 0;
			writes[0].dstSet = set;
			writes[0].pBufferInfo = &uboInfo;
			writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

			VkDescriptorBufferInfo ssboInfo = mIDSSBO[i]->GetDescriptorInfo();
			writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[1].descriptorCount = 1;
			writes[1].dstBinding = 1;
			writes[1].dstSet = set;
			writes[1].pBufferInfo = &ssboInfo;
			writes[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

			writes[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			writes[2].descriptorCount = 1;
			writes[2].dstBinding = 2;
			writes[2].dstSet = set;
			writes[2].pBufferInfo = &msdfLut;
			writes[2].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			
			vkUpdateDescriptorSets(mDevice, 3, writes, 0, nullptr);

			mBufferDescriptorSets[i] = set;
		}
	}

	void VulkanContext::RecycleTextDescriptor(VkDescriptorSet descriptor) {
		if (mOrphanedTextDescriptorSets.size() <= mMaxAllowedTextDescriptors / 4u) {
			mOrphanedTextDescriptorSets.push_back(descriptor);
			mStatistics.mRecycledDescriptors++;
		} else {
			FreeDescriptor(descriptor);
		}
	}

	ResourceObject VulkanContext::CreateTexture(const VkDescriptorImageInfo& imageInfo) {
		VkDescriptorSet set = {};
		VkDescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = mDescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &mTextureDescriptorSetLayout;
		assert(mStatistics.mTotalAllocatedDescriptors < SHUDVK_MAX_DESCRIPTOR_SETS && "Too many descriptor sets allocated!");
		if (VkResult result = vkAllocateDescriptorSets(mDevice, &allocateInfo, &set); result != VK_SUCCESS) {
			throw std::runtime_error(std::string("failed to allocate descriptor set, error code ") + string_VkResult(result));
		}
		mStatistics.mTotalAllocatedDescriptors++;
		mStatistics.mAllocatedTextures++;

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.dstBinding = 0;
		write.dstSet = set;
		write.pImageInfo = &imageInfo;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vkUpdateDescriptorSets(mDevice, 1, &write, 0, nullptr);

		ResourceObject res{};
		res._Set((void*)set);
		return res;
	}

	void VulkanContext::FreeTexture(const ResourceObject resource) {
		mSafeDestructionQueue[mLastImageIndex].push_back(std::make_shared<ResourceDestructionObject>(this, resource.Get(), [](void* context, const void* descriptor) { ((VulkanContext*)context)->FreeDescriptor((const VkDescriptorSet)descriptor); ((VulkanContext*)context)->mStatistics.mAllocatedTextures--; }));
	}

	static uint16_t ConvertToFloat16(float float_) {
		uint16_t fOutput = 0;
		unsigned int uiInput = reinterpret_cast<unsigned int&>(float_);

		if (0.0f == float_)
		{
			return 0;
		}

		else if (-0.0f == float_)
		{
			return 0x8000;
		}

		unsigned int uiSignBit = uiInput >> 31;
		unsigned int uiMantBits = (uiInput & 0x007FFFFF) >> 13;
		int  iExpBits = (uiInput >> 23) & 0xFF;

		//
		// Normalized values
		//

		int iExponent = iExpBits - 127 + 15;

		if (iExponent < 0) { iExponent = 0; } else if (iExponent > 31) iExponent = 31;

		fOutput = ((((uiSignBit) << 15) & 0x8000) | ((fOutput) & (0x7C00 | 0x03FF)));
		fOutput = ((((iExponent) << 10) & 0x7C00) | ((fOutput) & (0x8000 | 0x03FF)));
		fOutput = ((((uiMantBits) << 0) & 0x03FF) | ((fOutput) & (0x8000 | 0x7C00)));

		return fOutput;

	}


	BufferID VulkanContext::AllocateTextBuffer(size_t id, const std::string& characters, const TextFormatting& formatting) {
		assert(characters.size() <= 1024U && "Too many characters in this text!");
		mRequestedTextObjects.push_back(id);
		auto seek = mTextBuffers.find(id);
		if (seek != mTextBuffers.end()) {
			return (*seek).second->GetBufferID();
		}
		// TODO: recycle buffers and reuse them

		//FONT_FORMATTING_FLAG_BOLD =
		//	FONT_FORMATTING_FLAG_ITALIC
		MSDFTextData data;
		data.char_count = characters.length();
		data.fontSizePx = 16.0f;
		data.flags = 0;
		if (formatting.mBold) {
			data.flags |= FONT_FORMATTING_FLAG_BOLD;
		}
		if (formatting.mItalic) {
			data.flags |= FONT_FORMATTING_FLAG_ITALIC;
		}

		TextShaderBuffer* tbuffer = new TextShaderBuffer();
		mTextBuffers[id] = tbuffer;
		Buffer* charBuffer = new Buffer(
			mDevice, 
			mPhysicalDevice,
			sizeof(MSDFTextData) + std::min(uint64_t(ceil(characters.length() * 0.25f) * 4), 16000ui64),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		charBuffer->Map();
		charBuffer->WriteToBuffer(&data, sizeof(MSDFTextData));
		charBuffer->WriteToBuffer(characters.data(), std::min(characters.length(), 16000ui64), sizeof(MSDFTextData));
		charBuffer->Flush();
		mStatistics.mAllocatedTextBuffers++;

		struct offsets_ {
			fvec2 topleft;
			fvec2 bottomleft;
			fvec2 topright;
			fvec2 bottomright;
		};

		Buffer* offsetBuffer = new Buffer(
			mDevice,
			mPhysicalDevice,
			+ std::min(uint64_t(ceil(characters.length() * 0.25f) * 4) * sizeof(offsets_), 16384ui64),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);
		offsetBuffer->Map();

		std::vector<offsets_> offsets = std::vector<offsets_>(characters.length());
		for (int i = 0; i < offsets.size(); i++) {
			auto& uvfl = s3dfontdefault_uvlookup[static_cast<size_t>(characters[i])];
			offsets[i].topleft = { uvfl.planeBounds.x, uvfl.planeBounds.w };
			offsets[i].bottomleft = { uvfl.planeBounds.x, uvfl.planeBounds.y };
			offsets[i].topright = { uvfl.planeBounds.z, uvfl.planeBounds.w };
			offsets[i].bottomright = { uvfl.planeBounds.z, uvfl.planeBounds.y };

			if (i > 0) {
				offsets[i].topleft.x += offsets[i - 1].topright.x;
				offsets[i].bottomleft.x += offsets[i - 1].topright.x;
				offsets[i].topright.x += offsets[i - 1].topright.x;
				offsets[i].bottomright.x += offsets[i - 1].topright.x;
			}
		}
		std::vector<MSDFCharacterOffsetData> offsetShader = std::vector<MSDFCharacterOffsetData>(characters.length());
		float maxX = offsets.back().topright.x;
		float shift = 0.0;
		switch (formatting.mHAlignment) {
		case (TextFormattingHAlignment::Left): 
			shift = 0.0;
			break;
		case (TextFormattingHAlignment::Center):
			shift = -maxX / 2.0;
			break;
		case (TextFormattingHAlignment::Right):
			shift - maxX;
			break;
			// TODO: justified
		}
		for (int i = 0; i < offsetShader.size(); i++) {
			auto& offs = offsets[i];

			offsetShader[i].topleft_h[0] = ConvertToFloat16(offs.topleft[0] + shift);
			offsetShader[i].topleft_h[1] = ConvertToFloat16(offs.topleft[1]);
			offsetShader[i].bottomleft_h[0] = ConvertToFloat16(offs.bottomleft[0] + shift);
			offsetShader[i].bottomleft_h[1] = ConvertToFloat16(offs.bottomleft[1]);
			offsetShader[i].topright_h[0] = ConvertToFloat16(offs.topright[0] + shift);
			offsetShader[i].topright_h[1] = ConvertToFloat16(offs.topright[1]);
			offsetShader[i].bottomright_h[0] = ConvertToFloat16(offs.bottomright[0] + shift);
			offsetShader[i].bottomright_h[1] = ConvertToFloat16(offs.bottomright[1]);
		}

		offsetBuffer->WriteToBuffer(offsetShader.data(), std::min(offsetShader.size() * sizeof(MSDFCharacterOffsetData), 16384ui64));
		offsetBuffer->Flush();

		VkDescriptorSet set = {};
		if (mOrphanedTextDescriptorSets.size() > 0) {
			set = mOrphanedTextDescriptorSets.back();
			mOrphanedTextDescriptorSets.pop_back();
			mStatistics.mRecycledDescriptors--;
		} else {
			VkDescriptorSetAllocateInfo allocateInfo{};
			allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocateInfo.descriptorPool = mDescriptorPool;
			allocateInfo.descriptorSetCount = 1;
			allocateInfo.pSetLayouts = &mMSDFTextBufferDescriptorSetLayout;

			assert(mStatistics.mTotalAllocatedDescriptors < SHUDVK_MAX_DESCRIPTOR_SETS && "Too many descriptor sets allocated!");
			if (VkResult result = vkAllocateDescriptorSets(mDevice, &allocateInfo, &set); result != VK_SUCCESS) {
				throw std::runtime_error(std::string("failed to allocate descriptor set, error code ") + string_VkResult(result));
			}
			mStatistics.mTotalAllocatedDescriptors++;
		}

		auto charBufferInfo = charBuffer->GetDescriptorInfo();
		auto offsetBufferInfo = offsetBuffer->GetDescriptorInfo();
		VkWriteDescriptorSet writes[2]{};
		writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[0].descriptorCount = 1;
		writes[0].dstBinding = 0;
		writes[0].dstSet = set;
		writes[0].pBufferInfo = &charBufferInfo;
		writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[1].descriptorCount = 1;
		writes[1].dstBinding = 1;
		writes[1].dstSet = set;
		writes[1].pBufferInfo = &offsetBufferInfo;
		writes[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vkUpdateDescriptorSets(mDevice, 2, writes, 0, nullptr);

		tbuffer->_Set0(set);
		tbuffer->_Set1(charBuffer);
		tbuffer->_Set2(offsetBuffer);
		return tbuffer->GetBufferID();
	}

	void VulkanContext::Cleanup() {
		std::vector<size_t> remove;
		remove.reserve(mTextBuffers.size() / 2u);
		for (auto& [id, buffer] : mTextBuffers) {
			auto iter = std::find(mRequestedTextObjects.begin(), mRequestedTextObjects.end(), id);
			if (iter == mRequestedTextObjects.end()) {
				DeallocateTextBuffer(id);
				remove.push_back(id);
			}
		}
		for (size_t id : remove) {
			delete mTextBuffers.at(id);
			mTextBuffers.erase(id);
		}
		mRequestedTextObjects.clear();
		mSafeDestructionQueue[(mLastImageIndex + 1) % mSwapChainImageCount].clear();
	}

	void VulkanContext::DeallocateTextBuffer(size_t id) {
		TextShaderBuffer* sbObj = mTextBuffers.at(id);

		mSafeDestructionQueue[mLastImageIndex].push_back(std::make_shared<ResourceDestructionObject>(this, sbObj->Get0(), [](void* context, const void* descriptor) { ((VulkanContext*)context)->RecycleTextDescriptor((const VkDescriptorSet)descriptor); }));
		mSafeDestructionQueue[mLastImageIndex].push_back(std::make_shared<ResourceDestructionObject>(this, sbObj->Get1(), [](void* context, const void* buffer) { delete (const Buffer*)buffer; ((VulkanContext*)context)->mStatistics.mAllocatedTextBuffers--; }));
		mSafeDestructionQueue[mLastImageIndex].push_back(std::make_shared<ResourceDestructionObject>(this, sbObj->Get2(), [](void* context, const void* buffer) { delete (const Buffer*)buffer; }));
	}

	void VulkanContext::CreateResources(VkCommandBuffer singleTimeCommandBuffer) {
		LoadFontMSDF(singleTimeCommandBuffer);
	}
	void VulkanContext::FreeDescriptor(VkDescriptorSet descriptor) {
		vkFreeDescriptorSets(mDevice, mDescriptorPool, 1, &descriptor);
		mStatistics.mTotalAllocatedDescriptors--;
	}

	void VulkanContext::LoadFontMSDF(VkCommandBuffer singleTimeCommandBuffer) {
		VkDeviceSize imageSize = (sizeof(s3dfontdefault) / 3) * 4;

		mFontBuffer = new Buffer(
			mDevice,
			mPhysicalDevice,
			imageSize,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		std::vector<unsigned char> alignedData;
		alignedData.resize(imageSize);
		for (int i = 0; i < sizeof(s3dfontdefault); i+=3) {
			int stdIndex = i;
			int alignedIndex = (i / 3) * 4;
			alignedData[alignedIndex + 0] = s3dfontdefault[stdIndex + 0];
			alignedData[alignedIndex + 1] = s3dfontdefault[stdIndex + 1];
			alignedData[alignedIndex + 2] = s3dfontdefault[stdIndex + 2];
		}

		mFontBuffer->Map();
		memcpy(mFontBuffer->GetMappedMemory(), alignedData.data(), static_cast<size_t>(imageSize));
		mFontBuffer->Unmap();

		VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
		VkExtent3D extent = { 256, 256, 1 }; // atlas is 256x256px

		VkImageCreateInfo imageInfo{};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = VK_IMAGE_TYPE_2D;
		imageInfo.extent = extent;
		imageInfo.mipLevels = 1;
		imageInfo.arrayLayers = 1;
		imageInfo.format = format;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
			VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(mDevice, &imageInfo, nullptr, &mFontImage) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image!");
		}
		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(mDevice, mFontImage, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;

		VkPhysicalDeviceMemoryProperties memProperties;
		vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);
		for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
			if ((memRequirements.memoryTypeBits & (1 << i)) &&
				(memProperties.memoryTypes[i].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
				 allocInfo.memoryTypeIndex = i;
				 break;
			}
		}

		if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &mFontImageMemory) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate image memory!");
		}

		if (vkBindImageMemory(mDevice, mFontImage, mFontImageMemory, 0) != VK_SUCCESS) {
			throw std::runtime_error("failed to bind image memory!");
		}
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = mFontImage;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			
			vkCmdPipelineBarrier(
				singleTimeCommandBuffer,
				sourceStage,
				destinationStage,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier);
		}
		{
			VkBufferImageCopy region{};
			region.bufferOffset = 0;
			region.bufferRowLength = 0;
			region.bufferImageHeight = 0;

			region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			region.imageSubresource.mipLevel = 0;
			region.imageSubresource.baseArrayLayer = 0;
			region.imageSubresource.layerCount = 1;

			region.imageOffset = { 0, 0, 0 };
			region.imageExtent = extent;

			vkCmdCopyBufferToImage(
				singleTimeCommandBuffer,
				mFontBuffer->GetBuffer(),
				mFontImage,
				VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				1,
				&region
			);

		}
		{
			VkImageMemoryBarrier barrier{};
			barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

			barrier.image = mFontImage;
			barrier.subresourceRange.baseMipLevel = 0;
			barrier.subresourceRange.levelCount = 1;
			barrier.subresourceRange.baseArrayLayer = 0;
			barrier.subresourceRange.layerCount = 1;
			barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

			VkPipelineStageFlags sourceStage;
			VkPipelineStageFlags destinationStage;
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;

			vkCmdPipelineBarrier(
				singleTimeCommandBuffer,
				sourceStage,
				destinationStage,
				0,
				0,
				nullptr,
				0,
				nullptr,
				1,
				&barrier
			);
		}
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = mFontImage;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;
		viewInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		viewInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		viewInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		viewInfo.components.a = VK_COMPONENT_SWIZZLE_ZERO;

		if (vkCreateImageView(mDevice, &viewInfo, nullptr, &mFontImageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}
		
		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_LINEAR;
		samplerInfo.minFilter = VK_FILTER_LINEAR; 

		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;

		samplerInfo.anisotropyEnable = VK_FALSE;
		samplerInfo.maxAnisotropy = 1.0f;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;

		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_NEVER;

		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
		samplerInfo.mipLodBias = 0.0f;
		samplerInfo.minLod = 0.0f;
		samplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(mDevice, &samplerInfo, nullptr, &mFontSampler) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture sampler!");
		}

		VkDescriptorImageInfo descriptorInfo{};
		descriptorInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		descriptorInfo.imageView = mFontImageView;
		descriptorInfo.sampler = mFontSampler;

		mFontTexture = (VkDescriptorSet)CreateTexture(descriptorInfo).Get();
		mDefaultFontAtlas = (TextureID)mFontTexture;
	}
	void VulkanContext::CreateMSDFLUT() {
		mMSDFLUTBuffer = new Buffer(
			mDevice,
			mPhysicalDevice,
			sizeof(s3dfontdefault_uvlookup),
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
		);

		mMSDFLUTBuffer->Map();
		mMSDFLUTBuffer->WriteToBuffer(s3dfontdefault_uvlookup);
		mMSDFLUTBuffer->Unmap();

		// write garbage info to avoid validation errors from binding
		VkDescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = mDescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &mMSDFTextBufferDescriptorSetLayout;
		if (VkResult result = vkAllocateDescriptorSets(mDevice, &allocateInfo, &mDummyTextBuffer); result != VK_SUCCESS) {
			throw std::runtime_error(std::string("failed to allocate descriptor set, error code ") + string_VkResult(result));
		}
		mStatistics.mTotalAllocatedDescriptors++;

		auto bufferInfo = mMSDFLUTBuffer->GetDescriptorInfo();

		VkWriteDescriptorSet writes[2]{};

		writes[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[0].descriptorCount = 1;
		writes[0].dstBinding = 0;
		writes[0].dstSet = mDummyTextBuffer;
		writes[0].pBufferInfo = &bufferInfo;
		writes[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;

		writes[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writes[1].descriptorCount = 1;
		writes[1].dstBinding = 1;
		writes[1].dstSet = mDummyTextBuffer;
		writes[1].pBufferInfo = &bufferInfo;
		writes[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		vkUpdateDescriptorSets(mDevice, 2, writes, 0, nullptr);
	}


	void VulkanContext::DrawDebugInfo() {
		DrawList* drawList = GetDrawList();

		TextFormatting formatting;
		formatting.mBold = true;
		formatting.mItalic = false;
		formatting.mHAlignment = TextFormattingHAlignment::Left;
		formatting.mVAlignment = TextFormattingVAlignment::Bottom;
		formatting.mOutlineColor = 0x000000AA;
		formatting.mOutlineWidth = 4.0f;

		Transform dbgtrans;
		dbgtrans.mPosition = { 20, 50 };
		dbgtrans.mScale = { 32, 32 };
		dbgtrans.mTransformOffset = SHUD_ANCHOR_OFFSET_LEFT;

		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, "SHUDVK Statistics:", SHUD_ANCHOR_OFFSET_TOP_LEFT);
		formatting.mBold = false;
		dbgtrans.mPosition.y += 50;

		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "CPU Render Time: %.2f ms (Pot. %i FPS)",
			mStatistics.mCpuRenderTime * 1000.0f , static_cast<int>(1.0 / mStatistics.mCpuRenderTime) }, SHUD_ANCHOR_OFFSET_TOP_LEFT);

		dbgtrans.mPosition.y += 50;
		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "-Allocated Descriptors: %i", mStatistics.mTotalAllocatedDescriptors }, SHUD_ANCHOR_OFFSET_TOP_LEFT);
		dbgtrans.mPosition.y += 50;
		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "-Allocated Textures: %i", mStatistics.mAllocatedTextures }, SHUD_ANCHOR_OFFSET_TOP_LEFT);
		dbgtrans.mPosition.y += 50;
		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "-Recycled Descriptors: %i", mStatistics.mRecycledDescriptors }, SHUD_ANCHOR_OFFSET_TOP_LEFT);
		dbgtrans.mPosition.y += 50;
		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "-Draw Calls (prev frame): %i", mStatistics.mDrawCalls }, SHUD_ANCHOR_OFFSET_TOP_LEFT);
		dbgtrans.mPosition.y += 50;
		drawList->DrawText(dbgtrans, 0xFFFFFFAA, UINT16_MAX, formatting, { "-Vertices (prev frame): %i", mStatistics.mVertices }, SHUD_ANCHOR_OFFSET_TOP_LEFT);

	}

}