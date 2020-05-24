#pragma once

#include "vulkan/vulkan.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Memory
{
	/**
		As a performance improvement, it is better for Buffers to be allocated in chunks
		see https://developer.nvidia.com/vulkan-memory-management for details.
	*/
	struct VulkanBuffer
	{
		VkBuffer Buffer;
		VkDeviceMemory BufferMemory;
		VkBufferCreateInfo BufferCreateInfo;
	};

	struct BufferAllocInfo
	{
		size_t Size;
		VkBufferUsageFlags BufferUsageFlags;
		VkMemoryPropertyFlags MemoryPropertyFlags;
	};

	void VulkanBuffer_alloc(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, _Device::Device* p_device);
	void VulkanBuffer_free(VulkanBuffer* p_buffer, _Device::Device* p_device);
	void VulkanBuffer_pushToGPU(VulkanBuffer* p_buffer, _Device::Device* p_device, void* p_source, size_t p_size);
}