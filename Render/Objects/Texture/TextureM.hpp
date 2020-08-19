#pragma once

#include "Texture.hpp"

#include "DataStructures/Specifications/VectorT.hpp"
extern "C"
{
#include "v2/_interface/RectC_def.h"
}

namespace _RenderV2
{
	struct TextureM
	{
		inline static void allocPixels(Texture3C* p_texture)
		{
			_Core::ArrayT_alloc(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
			p_texture->Pixels.Size = p_texture->Pixels.Capacity;
		};

		inline static void freePixels(Texture3C* p_texture)
		{
			_Core::ArrayT_free(&p_texture->Pixels);
		};

		inline static size_t getSizeInBytes(const Texture3C* p_texture)
		{
			return ((size_t)p_texture->Width * p_texture->Height) * (3 * sizeof(char));
		};

		template <int N, typename T>
		inline static size_t getElementSize()
		{
			return N * sizeof(T);
		};

		inline static size_t getElementOffset(int W, int H, int textWidth, size_t elementSize)
		{
			return (W * elementSize) + (H * textWidth * elementSize);
		};

		inline static RECTI buildClipRect(Texture3C* p_texture)
		{
			return { {0,0}, {(int)p_texture->Width - 1, (int)p_texture->Height - 1} };
		};

		inline static TextureIterator3C buildIterator(Texture3C* p_texture)
		{
			return TextureIterator3C{p_texture, (VECTOR3C_PTR) ((char*)p_texture->Pixels.Memory - p_texture->Pixels.ElementSize), (size_t)-1};
		};
	};

	struct TextureIteratorM
	{
		inline static bool moveNext(TextureIterator3C* p_it)
		{
			p_it->Index += 1;
			p_it->Current = (VECTOR3C_PTR)((char*)p_it->Current + p_it->Texture->Pixels.ElementSize);
			return p_it->Index < p_it->Texture->Pixels.Size;
		}

		inline static void moveNextUnsafe(TextureIterator3C* p_it)
		{
			p_it->Index += 1;
			p_it->Current = (VECTOR3C_PTR)((char*)p_it->Current + p_it->Texture->Pixels.ElementSize);
		}
	};
}