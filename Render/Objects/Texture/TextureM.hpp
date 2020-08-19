#pragma once

#include "Texture.hpp"

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/VectorMath.hpp"
extern "C"
{
#include "v2/_interface/RectC_def.h"
}

namespace _RenderV2
{
	struct TextureM
	{
		template <int N, typename T>
		inline static void allocPixels(Texture<N, T>* p_texture)
		{
			_Core::ArrayT_alloc(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
			p_texture->Pixels.Size = p_texture->Pixels.Capacity;
		};

		template <int N, typename T>
		inline static void freePixels(Texture<N, T>* p_texture)
		{
			_Core::ArrayT_free(&p_texture->Pixels);
		};

		template <int N, typename T>
		inline static size_t getSizeInBytes(const Texture<N, T>* p_texture)
		{
			return ((size_t)p_texture->Width * p_texture->Height) * (N * sizeof(T));
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

		template <int N, typename T>
		inline static RECTI buildClipRect(Texture<N, T>* p_texture)
		{
			return { {0,0}, {(int)p_texture->Width - 1, (int)p_texture->Height - 1} };
		};

		template <int N, typename T>
		inline static TextureIterator<N, T> buildIterator(Texture<N, T>* p_texture)
		{
			return TextureIterator<N, T>{p_texture, (_MathV2::Vector<N, T>*) ((char*)p_texture->Pixels.Memory - p_texture->Pixels.ElementSize), (size_t)-1};
		};
	};

	struct TextureIteratorM
	{
		template <int N, typename T>
		inline static bool moveNext(TextureIterator<N, T>* p_it)
		{
			p_it->Index += 1;
			p_it->Current = (_MathV2::Vector<N, T>*)((char*)p_it->Current + p_it->Texture->Pixels.ElementSize);
			return p_it->Index < p_it->Texture->Pixels.Size;
		}

		template <int N, typename T>
		inline static void moveNextUnsafe(TextureIterator<N, T>* p_it)
		{
			p_it->Index += 1;
			p_it->Current = (_MathV2::Vector<N, T>*)((char*)p_it->Current + p_it->Texture->Pixels.ElementSize);
		}
	};
}