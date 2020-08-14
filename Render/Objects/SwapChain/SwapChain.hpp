#pragma once

#include "Objects/Texture/Texture.hpp"
#include "Functional/Callback/ObserverT.hpp"

namespace _RenderV2
{
	struct RenderV2Interface;
}

namespace _RenderV2
{
	struct SwapChain
	{
		RenderV2Interface* RenderInterface;
		Texture<3, char> PresentTexture;

		// The swap chain can be rebuild if the @ref MustBeRebuilt is set to true.
		// This event notify when such an event has occured.
		// /!\ WARNING - This observer is not cleaned up when the SwapChain is being rebuilded. It is up to program that register to it to unregister when they need it.
		_Core::ObserverT<RenderV2Interface> OnSwapChainBuilded;
	};

}