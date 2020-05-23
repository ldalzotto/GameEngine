#include "VertexBuffer.h"

#include <stdexcept>
#include "Log/Log.h"

namespace _GameEngine::_Render::_Mesh
{
	void createEmptyBuffer(VertexBuffer* p_vertexBuffer, size_t p_size, _Device::Device* p_device);
	void allocateBufferMemory(VertexBuffer* p_vertexBuffer, _Device::Device* p_device);

	void VertexBuffer_alloc(VertexBuffer* p_vertexBuffer, size_t p_size, _Device::Device* p_device)
	{
		createEmptyBuffer(p_vertexBuffer, p_size, p_device);
		allocateBufferMemory(p_vertexBuffer, p_device);
		vkBindBufferMemory(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->Buffer, p_vertexBuffer->VertexBufferMemory, 0);
	};

	void VertexBuffer_free(VertexBuffer* p_vertexBuffer, _Device::Device* p_device)
	{
		vkFreeMemory(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->VertexBufferMemory, nullptr);
		vkDestroyBuffer(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->Buffer, nullptr);
	};

	void createEmptyBuffer(VertexBuffer* p_vertexBuffer, size_t p_size, _Device::Device* p_device)
	{
		p_vertexBuffer->VkBufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		p_vertexBuffer->VkBufferCreateInfo.size = p_size;
		p_vertexBuffer->VkBufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		p_vertexBuffer->VkBufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(p_device->LogicalDevice.LogicalDevice, &p_vertexBuffer->VkBufferCreateInfo, nullptr, &p_vertexBuffer->Buffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to create vertex buffer!"));
		}
	};

	void allocateBufferMemory(VertexBuffer* p_vertexBuffer, _Device::Device* p_device)
	{
		VkMemoryRequirements l_memoryRequirements{};
		vkGetBufferMemoryRequirements(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->Buffer, &l_memoryRequirements);

		VkMemoryAllocateInfo l_memoryAllocateInfo{};
		l_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		l_memoryAllocateInfo.allocationSize = l_memoryRequirements.size;

		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is to be able to write to the buffer from CPU
		l_memoryAllocateInfo.memoryTypeIndex = _Device::Device_findMemoryType(p_device, l_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(p_device->LogicalDevice.LogicalDevice, &l_memoryAllocateInfo, nullptr, &p_vertexBuffer->VertexBufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Failed to allocate vertex buffer memory!"));
		}
	};

}