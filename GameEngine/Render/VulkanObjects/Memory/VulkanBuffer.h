#pragma once


#include "vulkan/vulkan.h"

namespace _GameEngine::_Render
{
	struct Device;
}

namespace _GameEngine::_Render
{

	struct BufferAllocInfo
	{
		size_t Size;
		VkBufferUsageFlags BufferUsageFlags;
		VkMemoryPropertyFlags MemoryPropertyFlags;
	};

	/**
		As a performance improvement, it is better for Buffers to be allocated in chunks
		see https://developer.nvidia.com/vulkan-memory-management for details.
	*/
	struct VulkanBuffer
	{
		VkBuffer Buffer;
		VkDeviceMemory BufferMemory;
		BufferAllocInfo BufferAllocInfo;
	};

	void VulkanBuffer_alloc(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device);
	void VulkanBuffer_free(VulkanBuffer* p_buffer, Device* p_device);
	void VulkanBuffer_map(VulkanBuffer* p_buffer, Device* p_device, void** out_address, size_t p_size);
	void VulkanBuffer_unMap(VulkanBuffer* p_buffer, Device* p_device);
	void VulkanBuffer_pushToGPU(VulkanBuffer* p_buffer, Device* p_device, void* p_source, size_t p_size);
}