#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/Vector.hpp"

namespace _GameEngine::_Math
{
	struct Box;
}

namespace _MathV2
{
	template<int C, int L, typename T>
	struct Matrix;
}

namespace _GameEngine::_Math
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

	void Box_build(Box* p_box, _Core::VectorT<_MathV2::Vector3<float>>* p_points);
	void Box_extractPoints(Box* p_box, BoxPoints* out_points);
	void Box_extractMinMax(Box* p_box, _MathV2::Vector3<float>& out_min, _MathV2::Vector3<float>& out_max);
	void BoxPoints_mul(BoxPoints* p_boxPoints, _MathV2::Matrix<4, 4, float>& p_matrix);
}