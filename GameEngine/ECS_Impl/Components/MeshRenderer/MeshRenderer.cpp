#include "MeshRenderer.h"

namespace _GameEngine::_ECS
{
	ComponentType MeshRendererType = "MeshRenderer";

	void MeshRenderer_free(void* p_meshRenderer, void* p_null);

	void MeshRenderer_init(MeshRenderer* p_meshRenderer, MeshRendererInitInfo* p_mehsRendererInfo)
	{
		p_meshRenderer->Render = p_mehsRendererInfo->Render;

		p_meshRenderer->OnComponentDetached.Closure = p_meshRenderer;
		p_meshRenderer->OnComponentDetached.Callback = MeshRenderer_free;
		_Utils::Observer_register(&p_mehsRendererInfo->AssociatedComponent->ComponentFreeEvent, &p_meshRenderer->OnComponentDetached);

		_Render::MeshAllocInfo l_meshAllocInfo{ };

		std::vector<_Render::Vertex> l_vertices = {
			{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
			{{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
		};
		std::vector<uint16_t> l_inidces = {
			 0, 1, 2, 2, 3, 0
		};

		l_meshAllocInfo.Device = &p_mehsRendererInfo->Render->Device;
		l_meshAllocInfo.PreRenderStagging = &p_mehsRendererInfo->Render->PreRenderStagging;
		l_meshAllocInfo.Vertices = &l_vertices;
		l_meshAllocInfo.Indices = &l_inidces;
		_Render::Mesh_alloc(&p_meshRenderer->Mesh, &l_meshAllocInfo);
		p_meshRenderer->MeshDrawCommand.Mesh = &p_meshRenderer->Mesh;

		// Descriptor set
		_Render::BufferAllocInfo l_bufferAllocInfo{};
		l_bufferAllocInfo.Size = sizeof(_Render::MeshUniformObject);
		l_bufferAllocInfo.BufferUsageFlags = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		l_bufferAllocInfo.MemoryPropertyFlags = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		_Render::VulkanBuffer_alloc(&p_meshRenderer->MeshDrawCommand.MeshUniformBuffer, &l_bufferAllocInfo, &p_mehsRendererInfo->Render->Device);

		VkDescriptorSetAllocateInfo l_descriptorSetAllocateInfo{};
		l_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		l_descriptorSetAllocateInfo.descriptorPool = p_meshRenderer->Render->DescriptorPool.DescriptorPool;
		l_descriptorSetAllocateInfo.descriptorSetCount = 1;
		l_descriptorSetAllocateInfo.pSetLayouts = &p_meshRenderer->Render->GraphicsPipeline.ShaderInputDescription.DescriptorSetLayout.DescriptorSetLayout;
		vkAllocateDescriptorSets(p_meshRenderer->Render->Device.LogicalDevice.LogicalDevice, &l_descriptorSetAllocateInfo, &p_meshRenderer->MeshDrawCommand.DescriptorSet);

		VkDescriptorBufferInfo l_descriptorUniformBufferInfo{};
		l_descriptorUniformBufferInfo.buffer = p_meshRenderer->MeshDrawCommand.MeshUniformBuffer.Buffer;
		l_descriptorUniformBufferInfo.offset = 0;
		l_descriptorUniformBufferInfo.range = p_meshRenderer->MeshDrawCommand.MeshUniformBuffer.BufferAllocInfo.Size;

		VkWriteDescriptorSet l_descriptorUniforBufferWrite{};
		l_descriptorUniforBufferWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		l_descriptorUniforBufferWrite.dstSet = p_meshRenderer->MeshDrawCommand.DescriptorSet;
		l_descriptorUniforBufferWrite.dstBinding = 0;
		l_descriptorUniforBufferWrite.dstArrayElement = 0;
		l_descriptorUniforBufferWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		l_descriptorUniforBufferWrite.descriptorCount = 1;
		l_descriptorUniforBufferWrite.pBufferInfo = &l_descriptorUniformBufferInfo;
		l_descriptorUniforBufferWrite.pImageInfo = nullptr;
		l_descriptorUniforBufferWrite.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(p_meshRenderer->Render->Device.LogicalDevice.LogicalDevice, 1, &l_descriptorUniforBufferWrite, 0, nullptr);
	};

	void MeshRenderer_updateMeshDrawUniform(MeshRenderer* p_meshRenderer, _Render::MeshUniformObject& l_meshUniformObject)
	{
		_Render::VulkanBuffer_pushToGPU(&p_meshRenderer->MeshDrawCommand.MeshUniformBuffer, &p_meshRenderer->Render->Device, &l_meshUniformObject, p_meshRenderer->MeshDrawCommand.MeshUniformBuffer.BufferAllocInfo.Size);
	};

	void MeshRenderer_free(void* p_meshRenderer, void* p_null)
	{
		MeshRenderer* l_meshRenderer = (MeshRenderer*)p_meshRenderer;
		_Render::Mesh_free(&l_meshRenderer->Mesh, &l_meshRenderer->Render->Device);
		_Render::VulkanBuffer_free(&l_meshRenderer->MeshDrawCommand.MeshUniformBuffer, &l_meshRenderer->Render->Device);
	};
}