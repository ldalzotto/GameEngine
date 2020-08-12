#pragma once

#include "Polygon.hpp"

namespace _RenderV2
{
	struct PolygonM
	{
		template <typename T>
		static T* get(Polygon<T>* p_polygon, short int p_index) 
		{
			return (T*)(((char*)p_polygon) + (sizeof(T) * p_index));
		};
	};

}