#pragma once

#include "Polygon.hpp"

#include "v2/Vector/Vector.hpp"

extern "C"
{
#include "v2/_interface/MatrixC.h"
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

		inline static Polygon<_MathV2::Vector<4, float>>* mul(const Polygon<_MathV2::Vector<4, float>>* p_polygon, const MATRIX4F_PTR p_matrix, Polygon<_MathV2::Vector<4, float>>* out_polygon)
		{
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v1, (VECTOR4F_PTR)&out_polygon->v1);
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v2, (VECTOR4F_PTR)&out_polygon->v2);
			Mat_Mul_M4F_V4F(p_matrix, (VECTOR4F_PTR)&p_polygon->v3, (VECTOR4F_PTR)&out_polygon->v3);
			return out_polygon;
		};

		inline static Polygon<_MathV2::Vector<4, float>>* mul_homogeneous(const Polygon<_MathV2::Vector<4, float>>* p_polygon, const MATRIX4F_PTR p_matrix, Polygon<_MathV2::Vector<4, float>>* out_polygon)
		{
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v1, (VECTOR4F_PTR)&out_polygon->v1);
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v2, (VECTOR4F_PTR)&out_polygon->v2);
			Mat_Mul_M4F_V4F_Homogeneous(p_matrix, (VECTOR4F_PTR)&p_polygon->v3, (VECTOR4F_PTR)&out_polygon->v3);
			return out_polygon;
		};

		template <int N, typename T>
		inline static Polygon<_MathV2::Vector<2, T>> cast_value_2(const Polygon<_MathV2::Vector<N, T>>* p_polygon)
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