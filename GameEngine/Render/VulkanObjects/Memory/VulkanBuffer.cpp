#include "VulkanBuffer.h"

#include <stdexcept>
#include "MYLog/MYLog.h"

#include "VulkanObjects/Hardware/Device/Device.h"

namespace _GameEngine::_Render
{
	void createEmptyBuffer(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device);
	void allocateBufferMemory(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device);

	void VulkanBuffer_alloc(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device)
	{
		createEmptyBuffer(p_buffer, p_bufferAllocInfo, p_device);
		allocateBufferMemory(p_buffer, p_bufferAllocInfo, p_device);
		vkBindBufferMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->Buffer, p_buffer->BufferMemory, 0);
	};

	void VulkanBuffer_free(VulkanBuffer* p_vertexBuffer, Device* p_device)
	{
			vkFreeMemory(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->BufferMemory, nullptr);
			vkDestroyBuffer(p_device->LogicalDevice.LogicalDevice, p_vertexBuffer->Buffer, nullptr);
			p_vertexBuffer->BufferMemory = VK_NULL_HANDLE;
			p_vertexBuffer->Buffer = VK_NULL_HANDLE;
	};


	void VulkanBuffer_map(VulkanBuffer* p_buffer, Device* p_device, void** out_address, size_t p_size)
	{
		vkMapMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->BufferMemory, 0, p_size, 0, out_address);
	};
	
	void VulkanBuffer_unMap(VulkanBuffer* p_buffer, Device* p_device)
	{
		vkUnmapMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->BufferMemory);
	};

	void VulkanBuffer_pushToGPU(VulkanBuffer* p_buffer, Device* p_device, void* p_source, size_t p_size)
	{
		void* data;
		vkMapMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->BufferMemory, 0, p_size, 0, &data);
		memcpy(data, p_source, p_size);
		vkUnmapMemory(p_device->LogicalDevice.LogicalDevice, p_buffer->BufferMemory);
	};

	void createEmptyBuffer(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device)
	{
		p_buffer->BufferAllocInfo = *p_bufferAllocInfo;

		VkBufferCreateInfo l_bufferCreateInfo{};
		l_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		l_bufferCreateInfo.size = p_bufferAllocInfo->Size;
		l_bufferCreateInfo.usage = p_bufferAllocInfo->BufferUsageFlags;
		l_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(p_device->LogicalDevice.LogicalDevice, &l_bufferCreateInfo, nullptr, &p_buffer->Buffer)
			!= VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to create vertex buffer!"));
		}
	};

	void allocateBufferMemory(VulkanBuffer* p_buffer, BufferAllocInfo* p_bufferAllocInfo, Device* p_device)
	{
		VkMemoryRequirements l_memoryRequirements{};
		vkGetBufferMemoryRequirements(p_device->LogicalDevice.LogicalDevice, p_buffer->Buffer, &l_memoryRequirements);

		VkMemoryAllocateInfo l_memoryAllocateInfo{};
		l_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		l_memoryAllocateInfo.allocationSize = l_memoryRequirements.size;

		// VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT is to be able to write to the buffer from CPU
		l_memoryAllocateInfo.memoryTypeIndex = Device_findMemoryType(p_device, l_memoryRequirements.memoryTypeBits, p_bufferAllocInfo->MemoryPropertyFlags);

		if (vkAllocateMemory(p_device->LogicalDevice.LogicalDevice, &l_memoryAllocateInfo, nullptr, &p_buffer->BufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Failed to allocate vertex buffer memory!"));
		}
	};

}