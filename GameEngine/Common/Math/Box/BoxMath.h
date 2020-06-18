#pragma once

#include "DataStructures/VectorT.h"
#include "Math/Vector/Vector.h"

namespace _GameEngine::_Math
{
	struct Box;
	struct Matrix4x4f;
}

namespace _GameEngine::_Math
{
	struct BoxPoints
	{
		_Math::Vector3f L_D_F;
		_Math::Vector3f L_U_F;

		_Math::Vector3f R_D_F;
		_Math::Vector3f R_U_F;
		
		_Math::Vector3f L_D_B;
		_Math::Vector3f L_U_B;
		
		_Math::Vector3f R_D_B;
		_Math::Vector3f R_U_B;
	};

	void Box_build(Box* p_box, _Core::VectorT<_Math::Vector3f>* p_points);
	void Box_extractPoints(Box* p_box, BoxPoints* out_points);
	void BoxPoints_mul(BoxPoints* p_boxPoints, Matrix4x4f* p_matrix);
}