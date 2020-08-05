#pragma once

#include "DeferredCommandBufferOperation.h"
#include "VulkanObjects/Memory/VulkanBuffer.h"
#include "Includes/VulkanIncludes.h"

namespace _GameEngine::_Render
{
	struct Device;
	struct Texture;
}

namespace _GameEngine::_Render
{
	struct TextureLoadDeferredOperation
	{
		VulkanBuffer SourceBuffer;
		Texture* Texture;
		Device* Device;
	};

	/**
		The @ref p_textureLoadDeferredOperation will be deleted when the @ref DeferredCommandBufferOperation is completed.
		See @ref TextureLoadDeferredOperation_onCompleted.
	*/
	DeferredCommandBufferOperation TextureLoadDeferredOperation_build(TextureLoadDeferredOperation** p_textureLoadDeferredOperation);
};