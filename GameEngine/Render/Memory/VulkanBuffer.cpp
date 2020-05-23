#include "VulkanBuffer.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_Memory
{
	void createEmptyBuffer(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, _Device::Device* p_device);
	void allocateBufferMemory(VulkanBuffer* p_buffer, _Device::Device* p_device);

	void VertexBuffer_alloc(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, _Device::Device* p_device)
	{
		createEmptyBuffer(p_buffer, p_bufferAllocInfo, p_device);
		allocateBufferMemory(p_buffer, p_device);
		vkBindBufferMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->Buffer, p_buffer->BufferMemory, 0);
	};

	void VertexBuffer_free(VulkanBuffer* p_vertexBuffer, _Device::Device* p_device)
	{
		vkFreeMemory(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->BufferMemory, nullptr);
		vkDestroyBuffer(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->Buffer, nullptr);
	};

	void createEmptyBuffer(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, _Device::Device* p_device)
	{
		p_buffer->BufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		p_buffer->BufferCreateInfo.size = p_bufferAllocInfo->Size;
		p_buffer->BufferCreateInfo.usage = p_bufferAllocInfo->BufferUsageFlags;
		p_buffer->BufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(p_device->LogicalDevice.LogicalDevice, &p_buffer->BufferCreateInfo, nullptr, &p_buffer->Buffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create vertex buffer!"));
		}
	};

	void allocateBufferMemory(VulkanBuffer* p_buffer, _Device::Device* p_device)
	{
		VkMemoryRequirements l_memoryRequirements{};
		vkGetBufferMemoryRequirements(p_device->LogicalDevice.LogicalDevice, p_buffer->Buffer, &l_memoryRequirements);

		VkMemoryAllocateInfo l_memoryAllocateInfo{};
		l_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		l_memoryAllocateInfo.allocationSize = l_memoryRequirements.size;

		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is to be able to write to the buffer from CPU
		l_memoryAllocateInfo.memoryTypeIndex = _Device::Device_findMemoryType(p_device, l_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(p_device->LogicalDevice.LogicalDevice, &l_memoryAllocateInfo, nullptr, &p_buffer->BufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate vertex buffer memory!"));
		}
	};

}