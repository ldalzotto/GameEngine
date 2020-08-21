#pragma once


#include "Objects/Texture/Texture_def.h"
#include "Functional/Callback/Observer_def.h"

typedef struct RenderV2Interface_TYP RenderV2Interface;

typedef Observer OnSwapChainBuildedObserver;

typedef struct SwapChain_TYP
{
	RenderV2Interface* RenderInterface;
	Texture3c PresentTexture;

	// The swap chain can be rebuild if the @ref MustBeRebuilt is set to true.
	// This event notify when such an event has occured.
	// /!\ WARNING - This observer is not cleaned up when the SwapChain is being rebuilded. It is up to program that register to it to unregister when they need it.
	OnSwapChainBuildedObserver /* _Core::ObserverT<RenderV2Interface> */ OnSwapChainBuilded;
}SwapChain, * SwapChain_PTR;
