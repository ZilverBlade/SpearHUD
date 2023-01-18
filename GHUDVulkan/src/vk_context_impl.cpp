#include <ghudvk/vk_context.h>
#include <ghudcpp/draw/draw_data.h>

namespace GHUD {
	VulkanContext::VulkanContext(const VulkanContextCreateInfo& createInfo) : m_Device(createInfo.m_Device), m_SwapChainImageCount(createInfo.m_SwapChainImageCount) {
		VkDescriptorPoolSize poolSizes[] = {
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1024 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 },
		};

		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		poolInfo.maxSets = 1024u + 1u + 1u;
		poolInfo.poolSizeCount = 1024u + 1u + 1u;
		poolInfo.pPoolSizes = poolSizes;
		vkCreateDescriptorPool(m_Device, &poolInfo, nullptr, &m_DescriptorPool);

		VkDescriptorSetLayoutBinding textureBinding{};
		textureBinding.binding = 0;
		textureBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		textureBinding.descriptorCount = 1;
		textureBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

		VkDescriptorSetLayoutCreateInfo textureLayoutCreateInfo{};
		textureLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		textureLayoutCreateInfo.bindingCount = 1;
		textureLayoutCreateInfo.pBindings = &textureBinding;
		vkCreateDescriptorSetLayout(m_Device, &textureLayoutCreateInfo, nullptr, &m_TextureDescriptorSetLayout);

		VkDescriptorSetLayoutBinding bufferBindings[2]{};
		bufferBindings[0].binding = 0;
		bufferBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		bufferBindings[0].descriptorCount = 1;
		bufferBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					  
		bufferBindings[1].binding = 1;
		bufferBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		bufferBindings[1].descriptorCount = 1;
		bufferBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

		VkDescriptorSetLayoutCreateInfo bufferLayoutCreateInfo{};
		bufferLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		bufferLayoutCreateInfo.bindingCount = 2;
		bufferLayoutCreateInfo.pBindings = bufferBindings;
		vkCreateDescriptorSetLayout(m_Device, &bufferLayoutCreateInfo, nullptr, &m_BufferDescriptorSetLayout);

		for (uint32 i = 0; i < createInfo.m_SwapChainImageCount; i++) {
			m_GlobalUBO.emplace_back(
				new Buffer(createInfo.m_Device, createInfo.m_PhysicalDevice, sizeof(GlobalContextInfo), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			);
			m_IDSSBO.emplace_back(
				new Buffer(createInfo.m_Device, createInfo.m_PhysicalDevice, sizeof(uint32), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
			);
			m_GlobalUBO[i]->map();
			m_IDSSBO[i]->map();
		}
	}

	VulkanContext::~VulkanContext() {
		for (uint32 i = 0; i < m_SwapChainImageCount; i++) {
			delete m_GlobalUBO[i];
			delete m_IDSSBO[i];
		}

		vkDestroyPipeline(m_Device, m_GraphicsPipeline, nullptr);
		vkDestroyPipelineLayout(m_Device, m_PipelineLayout, nullptr);

		vkDestroyDescriptorSetLayout(m_Device, m_TextureDescriptorSetLayout, nullptr);
		vkDestroyDescriptorSetLayout(m_Device, m_BufferDescriptorSetLayout, nullptr);
		vkDestroyDescriptorPool(m_Device, m_DescriptorPool, nullptr);
	}

	void VulkanContext::CreateGraphicsPipeline(const VulkanContextCreateInfo& createInfo) {
		static_assert(sizeof(DrawData) <= 128, "DrawData exceeds size of recommended vulkan spec push constant size of 128 bytes");

		VkPushConstantRange pushRange{};
		pushRange.offset = 0;
		pushRange.size = sizeof(DrawData);
		pushRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 2;
		pipelineLayoutInfo.pSetLayouts = nullptr; // global UBO and texture
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushRange; // push
		vkCreatePipelineLayout(m_Device, &pipelineLayoutInfo, nullptr, &m_PipelineLayout);

		std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
		shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
		shaderStages[0].module = nullptr; // shader module
		shaderStages[0].pName = "main";
		shaderStages[0].flags = 0;
		shaderStages[0].pNext = nullptr;
		shaderStages[0].pSpecializationInfo = nullptr;

		shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		shaderStages[1].module = nullptr; // shader module
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

		pipelineInfo.layout = m_PipelineLayout;
		pipelineInfo.renderPass = createInfo.m_RenderPass;
		pipelineInfo.subpass = createInfo.m_SubPass;

		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
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
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
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
		depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
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

		vkCreateGraphicsPipelines(m_Device, nullptr, 1, &pipelineInfo, nullptr, &m_GraphicsPipeline);
	}

	void VulkanContext::Render(VulkanFrameInfoStruct* frameInfoStruct) {
		VulkanFrameInfo& frameInfo = *reinterpret_cast<VulkanFrameInfo*>(frameInfoStruct);
		m_GlobalUBO[frameInfo.m_FrameIndex]->writeToBuffer(&m_CtxInfo);
		m_GlobalUBO[frameInfo.m_FrameIndex]->flush();
		vkCmdBindPipeline(frameInfo.m_CommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_GraphicsPipeline);
		vkCmdBindDescriptorSets(
			frameInfo.m_CommandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			m_PipelineLayout,
			0,
			1,
			&m_BufferDescriptorSet,
			0,
			nullptr
		);
		for (const auto& data : m_DrawList->GetList()) {
			if (data.m_Data->m_HasTexture == 1) {
				const VkDescriptorSet* textureDescriptor = reinterpret_cast<const VkDescriptorSet*>(&data.m_TextureID);
				vkCmdBindDescriptorSets(
					frameInfo.m_CommandBuffer,
					VK_PIPELINE_BIND_POINT_GRAPHICS,
					m_PipelineLayout,
					1,
					1,
					textureDescriptor,
					0,
					nullptr
				);
			}
			vkCmdPushConstants(frameInfo.m_CommandBuffer, m_PipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(DrawData), data.m_Data);
			vkCmdDraw(frameInfo.m_CommandBuffer, 6, 1, 0, 0);
		}
	}

	ResourceObject VulkanContext::CreateTexture(const VkDescriptorImageInfo& imageInfo) {
		VkDescriptorSet set = {};
		VkDescriptorSetAllocateInfo allocateInfo{};
		allocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocateInfo.descriptorPool = m_DescriptorPool;
		allocateInfo.descriptorSetCount = 1;
		allocateInfo.pSetLayouts = &m_TextureDescriptorSetLayout;
		vkAllocateDescriptorSets(m_Device, &allocateInfo, &set);

		VkWriteDescriptorSet write{};
		write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		write.descriptorCount = 1;
		write.dstBinding = 0;
		write.dstSet = set;
		write.pImageInfo = &imageInfo;
		write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		vkUpdateDescriptorSets(m_Device, 1, &write, 0, nullptr);
		return *reinterpret_cast<ResourceObject*>(&set);
	}
}