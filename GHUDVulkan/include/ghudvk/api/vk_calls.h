#pragma once
#include <ghudvk/api/vk_definitions.h>

//extern "C" {
//	extern PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
//	extern PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
//	extern PFN_vkAllocateMemory vkAllocateMemory;
//	extern PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
//	extern PFN_vkBindBufferMemory vkBindBufferMemory;
//	extern PFN_vkBindImageMemory vkBindImageMemory;
//	extern PFN_vkCmdBeginQuery vkCmdBeginQuery;
//	extern PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
//	extern PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
//	extern PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
//	extern PFN_vkCmdBindPipeline vkCmdBindPipeline;
//	extern PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
//	extern PFN_vkCmdBlitImage vkCmdBlitImage;
//	extern PFN_vkCmdClearAttachments vkCmdClearAttachments;
//	extern PFN_vkCmdClearColorImage vkCmdClearColorImage;
//	extern PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
//	extern PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
//	extern PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
//	extern PFN_vkCmdCopyImage vkCmdCopyImage;
//	extern PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
//	extern PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
//	extern PFN_vkCmdDispatch vkCmdDispatch;
//	extern PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
//	extern PFN_vkCmdDraw vkCmdDraw;
//	extern PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
//	extern PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
//	extern PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
//	extern PFN_vkCmdEndQuery vkCmdEndQuery;
//	extern PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
//	extern PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
//	extern PFN_vkCmdFillBuffer vkCmdFillBuffer;
//	extern PFN_vkCmdNextSubpass vkCmdNextSubpass;
//	extern PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
//	extern PFN_vkCmdPushConstants vkCmdPushConstants;
//	extern PFN_vkCmdResetEvent vkCmdResetEvent;
//	extern PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
//	extern PFN_vkCmdResolveImage vkCmdResolveImage;
//	extern PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
//	extern PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
//	extern PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
//	extern PFN_vkCmdSetEvent vkCmdSetEvent;
//	extern PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
//	extern PFN_vkCmdSetScissor vkCmdSetScissor;
//	extern PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
//	extern PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
//	extern PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
//	extern PFN_vkCmdSetViewport vkCmdSetViewport;
//	extern PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
//	extern PFN_vkCmdWaitEvents vkCmdWaitEvents;
//	extern PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
//	extern PFN_vkCreateBuffer vkCreateBuffer;
//	extern PFN_vkCreateBufferView vkCreateBufferView;
//	extern PFN_vkCreateCommandPool vkCreateCommandPool;
//	extern PFN_vkCreateComputePipelines vkCreateComputePipelines;
//	extern PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
//	extern PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
//	extern PFN_vkCreateDevice vkCreateDevice;
//	extern PFN_vkCreateEvent vkCreateEvent;
//	extern PFN_vkCreateFence vkCreateFence;
//	extern PFN_vkCreateFramebuffer vkCreateFramebuffer;
//	extern PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
//	extern PFN_vkCreateImage vkCreateImage;
//	extern PFN_vkCreateImageView vkCreateImageView;
//	extern PFN_vkCreateInstance vkCreateInstance;
//	extern PFN_vkCreatePipelineCache vkCreatePipelineCache;
//	extern PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
//	extern PFN_vkCreateQueryPool vkCreateQueryPool;
//	extern PFN_vkCreateRenderPass vkCreateRenderPass;
//	extern PFN_vkCreateSampler vkCreateSampler;
//	extern PFN_vkCreateSemaphore vkCreateSemaphore;
//	extern PFN_vkCreateShaderModule vkCreateShaderModule;
//	extern PFN_vkDestroyBuffer vkDestroyBuffer;
//	extern PFN_vkDestroyBufferView vkDestroyBufferView;
//	extern PFN_vkDestroyCommandPool vkDestroyCommandPool;
//	extern PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
//	extern PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
//	extern PFN_vkDestroyDevice vkDestroyDevice;
//	extern PFN_vkDestroyEvent vkDestroyEvent;
//	extern PFN_vkDestroyFence vkDestroyFence;
//	extern PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
//	extern PFN_vkDestroyImage vkDestroyImage;
//	extern PFN_vkDestroyImageView vkDestroyImageView;
//	extern PFN_vkDestroyInstance vkDestroyInstance;
//	extern PFN_vkDestroyPipeline vkDestroyPipeline;
//	extern PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
//	extern PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
//	extern PFN_vkDestroyQueryPool vkDestroyQueryPool;
//	extern PFN_vkDestroyRenderPass vkDestroyRenderPass;
//	extern PFN_vkDestroySampler vkDestroySampler;
//	extern PFN_vkDestroySemaphore vkDestroySemaphore;
//	extern PFN_vkDestroyShaderModule vkDestroyShaderModule;
//	extern PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
//	extern PFN_vkEndCommandBuffer vkEndCommandBuffer;
//	extern PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
//	extern PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
//	extern PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
//	extern PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
//	extern PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
//	extern PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
//	extern PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
//	extern PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
//	extern PFN_vkFreeMemory vkFreeMemory;
//	extern PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
//	extern PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
//	extern PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
//	extern PFN_vkGetDeviceQueue vkGetDeviceQueue;
//	extern PFN_vkGetEventStatus vkGetEventStatus;
//	extern PFN_vkGetFenceStatus vkGetFenceStatus;
//	extern PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
//	extern PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
//	extern PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
//	extern PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
//	extern PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
//	extern PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
//	extern PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
//	extern PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
//	extern PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
//	extern PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
//	extern PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
//	extern PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
//	extern PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
//	extern PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
//	extern PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
//	extern PFN_vkMapMemory vkMapMemory;
//	extern PFN_vkMergePipelineCaches vkMergePipelineCaches;
//	extern PFN_vkQueueBindSparse vkQueueBindSparse;
//	extern PFN_vkQueueSubmit vkQueueSubmit;
//	extern PFN_vkQueueWaitIdle vkQueueWaitIdle;
//	extern PFN_vkResetCommandBuffer vkResetCommandBuffer;
//	extern PFN_vkResetCommandPool vkResetCommandPool;
//	extern PFN_vkResetDescriptorPool vkResetDescriptorPool;
//	extern PFN_vkResetEvent vkResetEvent;
//	extern PFN_vkResetFences vkResetFences;
//	extern PFN_vkSetEvent vkSetEvent;
//	extern PFN_vkUnmapMemory vkUnmapMemory;
//	extern PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
//	extern PFN_vkWaitForFences vkWaitForFences;
//}