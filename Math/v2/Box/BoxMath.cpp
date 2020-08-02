#include "BoxMath.h"

#include "v2/Box/Box.hpp"

#include <math.h>
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

namespace _MathV2
{
	void box_grow(Box* p_box, const Vector3<float>* p_growingPoint)
	{
		Vector3<float> l_delta; VectorM::min(p_growingPoint, &p_box->Center, &l_delta);

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
		Vector3<float> l_approximateCenter{};
		{
			auto l_pointsIt = _Core::VectorT_buildIterator(p_points);
			while (_Core::VectorIteratorT_moveNext(&l_pointsIt))
			{
				VectorM::add(&l_approximateCenter, l_pointsIt.Current, &l_approximateCenter);
			}
			VectorM::mul(&l_approximateCenter, 1.0f / p_points->Size, &l_approximateCenter);
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
			p_box->Center = { 0.0f, 0.0f, 0.0f };

			VectorM::mul(
				VectorM::add(VectorM::add(&p_box->Center, &l_min, &p_box->Center), &l_max, &p_box->Center),
				0.5f,
				&p_box->Center
			);
		}

		box_grow(p_box, &l_min);
		box_grow(p_box, &l_max);
	};

	BoxPoints* Box_extractPoints(const Box* p_box, BoxPoints* p_out)
	{
		Vector3<float> tmp_vec3;
		p_out->Center = p_box->Center;
		// Set gizmo positions
		tmp_vec3 = { p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->R_U_F);
		tmp_vec3 = { p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->R_D_F);
		tmp_vec3 = { p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->R_U_B);
		tmp_vec3 = { p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->R_D_B);
		tmp_vec3 = { -p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->L_U_F);
		tmp_vec3 = { -p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->L_D_F);
		tmp_vec3 = { -p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->L_U_B);
		tmp_vec3 = { -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, &p_out->L_D_B);

		return p_out;
	};

	void Box_extractMinMax(const Box* p_box, _MathV2::Vector3<float>* out_min, _MathV2::Vector3<float>* out_max)
	{
		Vector3<float> tmp_vec3;
		tmp_vec3 = { -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, out_min);
		tmp_vec3 = { p_box->Extend.x, p_box->Extend.y, p_box->Extend.z }; VectorM::add(&p_box->Center, &tmp_vec3, out_max);
	};

	BoxPoints* BoxPoints_mul(const BoxPoints* p_boxPoints, const _MathV2::Matrix<4, 4, float>* p_matrix, BoxPoints* p_out)
	{
		_MathV2::Vector4<float> tmp_vec4;
		p_out->Center = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->Center, 1.0f), &tmp_vec4));
		p_out->L_D_B = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->L_D_B, 1.0f), &tmp_vec4));
		p_out->L_D_F = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->L_D_F, 1.0f), &tmp_vec4));
		p_out->L_U_B = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->L_U_B, 1.0f), &tmp_vec4));
		p_out->L_U_F = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->L_U_F, 1.0f), &tmp_vec4));
		p_out->R_D_B = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->R_D_B, 1.0f), &tmp_vec4));
		p_out->R_D_F = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->R_D_F, 1.0f), &tmp_vec4));
		p_out->R_U_B = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->R_U_B, 1.0f), &tmp_vec4));
		p_out->R_U_F = *VectorM::cast(MatrixM::mul(p_matrix, &VectorM::cast(&p_boxPoints->R_U_F, 1.0f), &tmp_vec4));
		return p_out;
	};

}