#pragma once

#include <stdint.h>
#include "v2/_interface/ColorC_def.h"

typedef struct TextureAssetHeader_TYP
{
	uint32_t ImageSizeInByte;
	uint16_t ImageWidth;
	uint16_t ImageHeight;
}TextureAssetHeader, *TextureAssetHeader_PTR;

typedef struct Texture3cAsset_TYP
{
	TextureAssetHeader Header;
	Color3c* Pixels;
}Texture3cAsset, * Texture3cAsset_PTR;


typedef struct Texture4cAsset_TYP
{
	TextureAssetHeader Header;
	Color4c* Pixels;
}Texture4cAsset, * Texture4cAsset_PTR;