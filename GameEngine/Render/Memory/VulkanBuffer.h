#pragma once

#include "vulkan/vulkan.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Memory
{
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
	};

	void VertexBuffer_alloc(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, _Device::Device* p_device);
	void VertexBuffer_free(VulkanBuffer* p_buffer, _Device::Device* p_device);
}