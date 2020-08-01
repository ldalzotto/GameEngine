#include "BoxMath.h"

#include <math.h>
#include "Math/Box/Box.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

using namespace _MathV2;

namespace _GameEngine::_Math
{
	void box_grow(Box* p_box, Vector3<float>& p_growingPoint)
	{
		Vector3<float> l_delta = VectorM::min(p_growingPoint, p_box->Center);

		if (fabs(l_delta.x) > p_box->Extend.x)
		{
			p_box->Extend.x = fabsf(l_delta.x);
		}
		if (fabs(l_delta.y) > p_box->Extend.y)
		{
			p_box->Extend.y = fabsf(l_delta.y);
		}
		if (fabs(l_delta.z) > p_box->Extend.z)
		{
			p_box->Extend.z = fabsf(l_delta.z);
		}
	};

	void Box_build(Box* p_box, _Core::VectorT<_MathV2::Vector3<float>>* p_points)
	{
		//Calculate approximate center
		Vector3<float> l_approximateCenter {};
		{
			auto l_pointsIt = _Core::VectorT_buildIterator(p_points);
			while (_Core::VectorIteratorT_moveNext(&l_pointsIt))
			{
				l_approximateCenter = VectorM::add(l_approximateCenter, *l_pointsIt.Current);
			}
			l_approximateCenter = VectorM::mul(l_approximateCenter, 1.0f / p_points->Size);
		}

		//Calculate min-max
		_MathV2::Vector3<float> l_min = l_approximateCenter, l_max = l_approximateCenter;
		{
			_Core::VectorIteratorT<_MathV2::Vector3<float>> l_pointsIt = _Core::VectorT_buildIterator(p_points);
			while (_Core::VectorIteratorT_moveNext(&l_pointsIt))
			{
				if (l_pointsIt.Current->x <= l_min.x) { l_min.x = l_pointsIt.Current->x; }
				if (l_pointsIt.Current->y <= l_min.y) { l_min.y = l_pointsIt.Current->y; }
				if (l_pointsIt.Current->z <= l_min.z) { l_min.z = l_pointsIt.Current->z; }


				if (l_pointsIt.Current->x >= l_max.x) { l_max.x = l_pointsIt.Current->x; }
				if (l_pointsIt.Current->y >= l_max.y) { l_max.y = l_pointsIt.Current->y; }
				if (l_pointsIt.Current->z >= l_max.z) { l_max.z = l_pointsIt.Current->z; }
			}
		}

		//calculate accurate center
		{
			p_box->Center =
				VectorM::mul(
					VectorM::add(VectorM::add({ 0.0f, 0.0f, 0.0f }, l_min), l_max),
					0.5f
				);
		}

		box_grow(p_box, l_min);
		box_grow(p_box, l_max);
	};

	void Box_extractPoints(Box* p_box, BoxPoints* out_points)
	{
		out_points->Center = p_box->Center;
		// Set gizmo positions
		out_points->R_U_F = VectorM::add(p_box->Center, { p_box->Extend.x, p_box->Extend.y, p_box->Extend.z });
		out_points->R_D_F = VectorM::add(p_box->Center, { p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z });
		out_points->R_U_B = VectorM::add(p_box->Center, { p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z });
		out_points->R_D_B = VectorM::add(p_box->Center, { p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z });
		out_points->L_U_F = VectorM::add(p_box->Center, { -p_box->Extend.x, p_box->Extend.y, p_box->Extend.z });
		out_points->L_D_F = VectorM::add(p_box->Center, { -p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z });
		out_points->L_U_B = VectorM::add(p_box->Center, { -p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z });
		out_points->L_D_B = VectorM::add(p_box->Center, { -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z });
	};

	void Box_extractMinMax(Box* p_box, _MathV2::Vector3<float>& out_min, _MathV2::Vector3<float>& out_max)
	{
		// min
		out_min = VectorM::add(p_box->Center, { -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z });
		out_max = VectorM::add(p_box->Center, { p_box->Extend.x, p_box->Extend.y, p_box->Extend.z });
	};

	void BoxPoints_mul(BoxPoints* p_boxPoints, const _MathV2::Matrix<4, 4, float>& p_matrix)
	{
		p_boxPoints->Center = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->Center, 1.0f)));
		p_boxPoints->L_D_B = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->L_D_B, 1.0f)));
		p_boxPoints->L_D_F = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->L_D_F, 1.0f)));
		p_boxPoints->L_U_B = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->L_U_B, 1.0f)));
		p_boxPoints->L_U_F = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->L_U_F, 1.0f)));
		p_boxPoints->R_D_B = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->R_D_B, 1.0f)));
		p_boxPoints->R_D_F = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->R_D_F, 1.0f)));
		p_boxPoints->R_U_B = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->R_U_B, 1.0f)));
		p_boxPoints->R_U_F = VectorM::cast(MatrixM::mul(p_matrix, VectorM::cast(p_boxPoints->R_U_F, 1.0f)));
	};

}