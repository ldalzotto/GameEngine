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
}TextureAsset, * TextureAsset_PTR;