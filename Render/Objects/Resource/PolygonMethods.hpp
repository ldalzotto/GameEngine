#pragma once

#include "Polygon.hpp"

extern "C"
{
#include "v2/_interface/MatrixC.h"
#include "v2/_interface/VectorC_def.h"
}

namespace _RenderV2
{
	struct PolygonM
	{
		template <typename T>
		inline static T* get(Polygon<T>* p_polygon, short int p_index)
		{
			return (T*)(((char*)p_polygon) + (sizeof(T) * p_index));
		};

		inline static Polygon<VECTOR4F>* mul(const Polygon<VECTOR4F>* p_polygon, const MATRIX4F_PTR p_matrix, Polygon<VECTOR4F>* out_polygon)
		{
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v1, &out_polygon->v1);
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v2, &out_polygon->v2);
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v3, &out_polygon->v3);
			return out_polygon;
		};

		inline static Polygon<VECTOR4F>* mul_homogeneous(const Polygon<VECTOR4F>* p_polygon, const MATRIX4F_PTR p_matrix, Polygon<VECTOR4F>* out_polygon)
		{
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v1, &out_polygon->v1);
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v2, &out_polygon->v2);
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v3, &out_polygon->v3);
			return out_polygon;
		};

		inline static Polygon<VECTOR2F> cast_value_2(const Polygon<VECTOR2F>* p_polygon)
		{
			return
			{
				{p_polygon->v1.x, p_polygon->v1.y},
				{p_polygon->v2.x, p_polygon->v2.y},
				{p_polygon->v3.x, p_polygon->v3.y}
			};
		};
	};

}