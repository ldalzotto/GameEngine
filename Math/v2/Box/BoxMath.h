#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/Vector.hpp"

namespace _MathV2
{
	struct Box;

	template<int C, int L, typename T>
	struct Matrix;
}

namespace _MathV2
{
	struct BoxPoints
	{
		_MathV2::Vector3<float> L_D_F;
		_MathV2::Vector3<float> L_U_F;

		_MathV2::Vector3<float> R_D_F;
		_MathV2::Vector3<float> R_U_F;
		
		_MathV2::Vector3<float> L_D_B;
		_MathV2::Vector3<float> L_U_B;
		
		_MathV2::Vector3<float> R_D_B;
		_MathV2::Vector3<float> R_U_B;

		_MathV2::Vector3<float> Center;
	};

	void Box_build(Box& p_box, _Core::VectorT<_MathV2::Vector3<float>>& p_points);
	BoxPoints Box_extractPoints(const Box& p_box);
	void Box_extractMinMax(const Box& p_box, _MathV2::Vector3<float>* out_min, _MathV2::Vector3<float>* out_max);
	BoxPoints BoxPoints_mul(const BoxPoints& p_boxPoints, const _MathV2::Matrix<4, 4, float>& p_matrix);
}