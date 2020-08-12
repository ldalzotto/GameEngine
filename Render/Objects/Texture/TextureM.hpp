#pragma once

#include "Texture.hpp"
#include "RTexture.hpp"

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/VectorMath.hpp"

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
		inline static void fill(Texture<N, T>* p_texture, const _MathV2::Vector<N, T>* p_color)
		{
			RTexture l_texture = { (char*)p_texture->Pixels.Memory, p_texture->Width, p_texture->Height, getElementSize<N, T>() };
			RTexture_fill(&l_texture, (const T*)p_color);
		};

		template <int N, typename T>
		inline static _MathV2::Vector<N, T>* getPixel(Texture<N, T>* p_texture, int W, int H)
		{
			return _Core::ArrayT_at(&p_texture->Pixels, ((size_t)H * p_texture->Width) + W);
		};

		template <int N, typename T>
		inline static void writePixels(Texture<N, T>* p_texture, const _Core::VectorT<_MathV2::Vector<2, int>>* p_coordinates, const _Core::VectorT<_MathV2::Vector<N, T>>* p_colors)
		{
			RTexture l_texture = { (char*)p_texture->Pixels.Memory, p_texture->Width, p_texture->Height, getElementSize<N, T>() };
			RTexture_drawPixels(&l_texture, (const RTexturePixelCoordinates*)p_coordinates->Memory, (const void*)p_colors->Memory, p_coordinates->Size);
		};

		template <int N, typename T>
		inline static void writePixel(Texture<N, T>* p_texture, const _MathV2::Vector<2, int>* p_coordinate, const _MathV2::Vector<N, T>* p_color)
		{
			RTexture l_texture = { (char*)p_texture->Pixels.Memory, p_texture->Width, p_texture->Height, getElementSize<N, T>() };
			RTexture_drawPixel(&l_texture, (const RTexturePixelCoordinates*)p_coordinate, (const void*)p_color);
		};
	};
}