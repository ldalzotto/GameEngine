#pragma once

#include "vulkan/vulkan.h"
 
namespace _GameEngine::_Render
{
	struct Render;

	struct RenderHookCallbacks;
	struct BeforeEndRecordingMainCommandBuffer_Input;

	struct Window;
	struct Surface;
	struct ValidationLayers;
	struct RenderDebug;
	struct Device;
	struct SwapChain;
	struct Texture;
	struct CommandPool;
	struct CommandBuffer;
	struct RenderSemaphore;
	struct TextureSamplers;

	struct TextureResourceProvider;
	struct MeshResourceProvider;

	struct DefaultMaterialV2;

	struct PreRenderDeferedCommandBufferStep;
	struct CameraBufferSetupStep;
	struct DefaultMaterialDrawStep;
}

namespace _GameEngine::_Render
{
	 
	struct RenderMaterialsInterface
	{
		DefaultMaterialV2* DefaultMaterial;
	};

	struct ResourceProvidersInterface
	{
		TextureResourceProvider* TextureResourceProvider;
		MeshResourceProvider* MeshResourceProvider;
	};

	struct BeforeEndRecordingMainCommandBuffer_InputInterface
	{
		BeforeEndRecordingMainCommandBuffer_Input* BeforeEndRecordingMainCommandBuffer_Input;
	};

	struct RenderHookCallbacksInterface
	{
		RenderHookCallbacks* RenderHookCallbacks;
	};

	struct RenderInterface
	{
		RenderHookCallbacksInterface RenderHookCallbacksInterface;

		Window* Window;
		Surface* WindowSurface;
		VkInstance* Instance;
		ValidationLayers* ValidationLayers;
		RenderDebug* RenderDebug;
		Device* Device;
		SwapChain* SwapChain;
		Texture** DepthTexture;
		CommandPool* CommandPool;
		RenderSemaphore* RenderSemaphore;
		TextureSamplers* TextureSamplers;

		ResourceProvidersInterface ResourceProvidersInterface;
		RenderMaterialsInterface RenderMaterialsInterface;

		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
		CameraBufferSetupStep* CameraBufferSetupStep;
		DefaultMaterialDrawStep* DefaultMaterialDrawStep;
	};

	void RenderInterface_initialize(Render* p_render);
}