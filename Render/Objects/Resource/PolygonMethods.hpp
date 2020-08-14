#pragma once

#include "Polygon.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/Vector.hpp"

namespace _RenderV2
{
	struct PolygonM
	{
		template <typename T>
		inline static T* get(Polygon<T>* p_polygon, short int p_index) 
		{
			return (T*)(((char*)p_polygon) + (sizeof(T) * p_index));
		};

		template <int N, typename T>
		inline static Polygon<_MathV2::Vector<N, T>>* mul(const Polygon<_MathV2::Vector<N, T>>* p_polygon, _MathV2::Matrix<N, N, T>* p_matrix, Polygon<_MathV2::Vector<N, T>>* out_polygon)
		{
			_MathV2::MatrixM::mul(p_matrix, &p_polygon->v1, &out_polygon->v1);
			_MathV2::MatrixM::mul(p_matrix, &p_polygon->v2, &out_polygon->v2);
			_MathV2::MatrixM::mul(p_matrix, &p_polygon->v3, &out_polygon->v3);
			return out_polygon;
		};

		template <int N, typename T>
		inline static Polygon<_MathV2::Vector<N, T>>* mul_homogeneous(const Polygon<_MathV2::Vector<N, T>>* p_polygon, _MathV2::Matrix<N, N, T>* p_matrix, Polygon<_MathV2::Vector<N, T>>* out_polygon)
		{
			_MathV2::MatrixM::mul_homogeneous(p_matrix, &p_polygon->v1, &out_polygon->v1);
			_MathV2::MatrixM::mul_homogeneous(p_matrix, &p_polygon->v2, &out_polygon->v2);
			_MathV2::MatrixM::mul_homogeneous(p_matrix, &p_polygon->v3, &out_polygon->v3);
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