#include "FrustumMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"

extern "C"
{
#include "v2/_interface/PlaneC.h"
}

#include "v2/Vector/VectorMath.hpp"

namespace _MathV2
{
	// Reference  http://deptinfo.unice.fr/twiki/pub/Minfo03/AlgosJeux3DMariani/planeextraction.pdf
	Frustum FrustumM::extractFrustumFromProjection(Matrix<4, 4, float>* p_projection)
	{
		Frustum l_frustum;

		//TODO - Having a more direct way instead of calling inv of projection ? (more efficient - see link above)
#if 1
		Matrix<4, 4, float> l_projection_inverted;
		MatrixM::inv(p_projection, &l_projection_inverted);

		Vector4<float> l_right_up_far, l_right_up_near, l_right_bottom_far, l_right_bottom_near;
		Vector4<float> l_left_up_far, l_left_up_near, l_left_bottom_far, l_left_bottom_near;

		Vector4<float> tmp_vec4_0 = { 1.0f, 1.0f, -1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_up_far);
		VectorM::mul(&l_right_up_far, 1.0f / l_right_up_far.w, &l_right_up_far);

		tmp_vec4_0 = { 1.0f, 1.0f, 1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_up_near);
		VectorM::mul(&l_right_up_near, 1.0f / l_right_up_near.w, &l_right_up_near);

		tmp_vec4_0 = { 1.0f, -1.0f, -1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_far);
		VectorM::mul(&l_right_bottom_far, 1.0f / l_right_bottom_far.w, &l_right_bottom_far);

		tmp_vec4_0 = { 1.0f, -1.0f, 1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_near);
		VectorM::mul(&l_right_bottom_near, 1.0f / l_right_bottom_near.w, &l_right_bottom_near);


		tmp_vec4_0 = { -1.0f, 1.0f, -1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_up_far);
		VectorM::mul(&l_left_up_far, 1.0f / l_left_up_far.w, &l_left_up_far);

		tmp_vec4_0 = { -1.0f, 1.0f, 1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_up_near);
		VectorM::mul(&l_left_up_near, 1.0f / l_left_up_near.w, &l_left_up_near);

		tmp_vec4_0 = { -1.0f, -1.0f, -1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_far);
		VectorM::mul(&l_left_bottom_far, 1.0f / l_left_bottom_far.w, &l_left_bottom_far);

		tmp_vec4_0 = { -1.0f, -1.0f, 1.0f, -1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_near);
		VectorM::mul(&l_left_bottom_near, 1.0f / l_left_bottom_near.w, &l_left_bottom_near);


		Plane_Build_3Points((VECTOR3F_PTR)&l_left_bottom_near, (VECTOR3F_PTR)&l_left_bottom_far, (VECTOR3F_PTR)&l_left_up_near, &l_frustum.Left);
		Plane_Build_3Points((VECTOR3F_PTR)&l_right_bottom_near, (VECTOR3F_PTR)&l_right_up_near, (VECTOR3F_PTR)&l_right_bottom_far, &l_frustum.Right);

		Plane_Build_3Points((VECTOR3F_PTR)&l_left_bottom_near, (VECTOR3F_PTR)&l_right_bottom_near, (VECTOR3F_PTR)&l_left_bottom_far, &l_frustum.Bottom);
		Plane_Build_3Points((VECTOR3F_PTR)&l_left_up_near, (VECTOR3F_PTR)&l_left_up_far, (VECTOR3F_PTR)&l_right_up_near, &l_frustum.Up);

		Plane_Build_3Points((VECTOR3F_PTR)&l_left_bottom_near, (VECTOR3F_PTR)&l_left_up_near, (VECTOR3F_PTR)&l_right_bottom_near, &l_frustum.Near);
		Plane_Build_3Points((VECTOR3F_PTR)&l_left_bottom_far, (VECTOR3F_PTR)&l_right_bottom_far, (VECTOR3F_PTR)&l_left_up_far, &l_frustum.Far);

#endif


		return l_frustum;
	};
}