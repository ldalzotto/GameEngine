#pragma once

#include "vulkan/vulkan.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Mesh
{
	struct VertexBuffer
	{
		VkBuffer Buffer;
		VkDeviceMemory VertexBufferMemory;
		VkBufferCreateInfo VkBufferCreateInfo;
	};

	void VertexBuffer_alloc(VertexBuffer* p_vertexBuffer, size_t p_size, _Device::Device* p_device);
	void VertexBuffer_free(VertexBuffer* p_vertexBuffer, _Device::Device* p_device);
}