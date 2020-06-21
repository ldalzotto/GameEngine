#include "BoxMath.h"

#include <math.h>
#include "Math/Box/Box.h"
#include "Math/Vector/VectorMath.h"
#include "Math/Matrix/MatrixMath.h"

namespace _GameEngine::_Math
{
	void box_grow(Box* p_box, Vector3f* p_growingPoint)
	{
		Vector3f l_delta;
		{
			Vector3f_min(p_growingPoint, &p_box->Center, &l_delta);
		}

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

	void Box_build(Box* p_box, _Core::VectorT<_Math::Vector3f>* p_points)
	{
		//calculate center
		{
			p_box->Center = { 0.0f, 0.0f, 0.0f };
			for (size_t i = 0; i < p_points->size(); i++)
			{
				_Math::Vector3f_add(&p_box->Center, p_points->at(i), &p_box->Center);
			}
			_Math::Vector3f_mul(&p_box->Center, 1.0f / p_points->size(), &p_box->Center);
		}

		{
			for (size_t i = 0; i < p_points->size(); i++)
			{
				box_grow(p_box, p_points->at(i));
			}
		}
	};

	void Box_extractPoints(Box* p_box, BoxPoints* out_points)
	{
		out_points->Center = p_box->Center;
		// Set gizmo positions
		{
			out_points->R_U_F = p_box->Center;
			_Math::Vector3f l_add = { p_box->Extend.x, p_box->Extend.y, p_box->Extend.z };
			_Math::Vector3f_add(&out_points->R_U_F, &l_add, &out_points->R_U_F);
		}
		{
			out_points->R_D_F = p_box->Center;
			_Math::Vector3f l_add = { p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z };
			_Math::Vector3f_add(&out_points->R_D_F, &l_add, &out_points->R_D_F);
		}
		{
			out_points->R_U_B = p_box->Center;
			_Math::Vector3f l_add = { p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z };
			_Math::Vector3f_add(&out_points->R_U_B, &l_add, &out_points->R_U_B);
		}
		{
			out_points->R_D_B = p_box->Center;
			_Math::Vector3f l_add = { p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z };
			_Math::Vector3f_add(&out_points->R_D_B, &l_add, &out_points->R_D_B);
		}

		{
			out_points->L_U_F = p_box->Center;
			_Math::Vector3f l_add = { -p_box->Extend.x, p_box->Extend.y, p_box->Extend.z };
			_Math::Vector3f_add(&out_points->L_U_F, &l_add, &out_points->L_U_F);
		}
		{
			out_points->L_D_F = p_box->Center;
			_Math::Vector3f l_add = { -p_box->Extend.x, -p_box->Extend.y, p_box->Extend.z };
			_Math::Vector3f_add(&out_points->L_D_F, &l_add, &out_points->L_D_F);
		}
		{
			out_points->L_U_B = p_box->Center;
			_Math::Vector3f l_add = { -p_box->Extend.x, p_box->Extend.y, -p_box->Extend.z };
			_Math::Vector3f_add(&out_points->L_U_B, &l_add, &out_points->L_U_B);
		}
		{
			out_points->L_D_B = p_box->Center;
			_Math::Vector3f l_add = { -p_box->Extend.x, -p_box->Extend.y, -p_box->Extend.z };
			_Math::Vector3f_add(&out_points->L_D_B, &l_add, &out_points->L_D_B);
		}

	};

	void BoxPoints_mul(BoxPoints* p_boxPoints, Matrix4x4f* p_matrix)
	{
		_Math::Vector4f l_pointAsVec4;
		_Math::Vector4f l_transformedPoint;

		{
			_Math::Vector4f_build(&p_boxPoints->Center, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->Center = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}

		{
			_Math::Vector4f_build(&p_boxPoints->L_D_B, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->L_D_B = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
		{
			_Math::Vector4f_build(&p_boxPoints->L_D_F, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->L_D_F = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
		{
			_Math::Vector4f_build(&p_boxPoints->L_U_B, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->L_U_B = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		} 
		{
			_Math::Vector4f_build(&p_boxPoints->L_U_F, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->L_U_F = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}

		{
			_Math::Vector4f_build(&p_boxPoints->R_D_B, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->R_D_B = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
		{
			_Math::Vector4f_build(&p_boxPoints->R_D_F, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->R_D_F = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
		{
			_Math::Vector4f_build(&p_boxPoints->R_U_B, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->R_U_B = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
		{
			_Math::Vector4f_build(&p_boxPoints->R_U_F, 1.0f, &l_pointAsVec4);
			_Math::Matrixf4x4_mul(p_matrix, &l_pointAsVec4, &l_transformedPoint);
			p_boxPoints->R_U_F = { l_transformedPoint.x, l_transformedPoint.y, l_transformedPoint.z };
		}
	};

}