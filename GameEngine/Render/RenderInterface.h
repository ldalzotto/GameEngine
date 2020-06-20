#pragma once

#include "vulkan/vulkan.h"

namespace _GameEngine::_Log
{
	struct MyLog;
}

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
	struct TextureSwapChainSizeSynchronizer;
	struct MaterialInstanceContainer;

	struct TextureResourceProvider;
	struct MeshResourceProvider;
	struct MaterialResourceProvider;

	struct PreRenderDeferedCommandBufferStep;
	struct CameraBufferSetupStep;
	struct DefaultMaterialDrawStep;
	struct Gizmo;
}

namespace _GameEngine::_Render
{
	struct ResourceProvidersInterface
	{
		TextureResourceProvider* TextureResourceProvider;
		MeshResourceProvider* MeshResourceProvider;
		MaterialResourceProvider* MaterialResourceProvider;
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
		_Log::MyLog* MyLog;

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
		TextureSwapChainSizeSynchronizer* TextureSwapChainSizeSynchronizer;
		MaterialInstanceContainer* MaterialInstanceContainer;

		ResourceProvidersInterface ResourceProvidersInterface;

		PreRenderDeferedCommandBufferStep* PreRenderDeferedCommandBufferStep;
		CameraBufferSetupStep* CameraBufferSetupStep;
		DefaultMaterialDrawStep* DefaultMaterialDrawStep;
		Gizmo* Gizmo;
	};

	void RenderInterface_initialize(Render* p_render, _Log::MyLog* p_myLog);
}