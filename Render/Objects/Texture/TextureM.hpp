#pragma once

#include "Texture.hpp"
#include "RTexture.hpp"

#include "v2/Vector/VectorMath.hpp"

namespace _RenderV2
{
	struct TextureM
	{
		template <int N, typename T>
		inline static void allocPixels(Texture<N, T>* p_texture)
		{
			_Core::ArrayT_alloc(&p_texture->Pixels, ((size_t)p_texture->Width * p_texture->Height));
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
		inline static void fill(Texture<N, T>* p_texture, const _MathV2::Vector<N, T>* p_color)
		{
			RTexture_fill((char*)p_texture->Pixels.Memory, (const T*)p_color, sizeof(T) * N, (size_t)p_texture->Width * p_texture->Height);
		};

	};
}