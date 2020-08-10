#include "FrustumMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Plane/PlaneMath.hpp"
#include "v2/Vector/VectorMath.hpp"

namespace _MathV2
{
	// Reference  http://deptinfo.unice.fr/twiki/pub/Minfo03/AlgosJeux3DMariani/planeextraction.pdf
	Frustum FrustumM::extractFrustumFromProjection(Matrix<4, 4, float>* p_projection)
	{
		Frustum l_frustum;

#if 0
		l_frustum.Left = PlaneM::build(
			p_projection->Points[0][3] + p_projection->Points[0][0],
			p_projection->Points[1][3] + p_projection->Points[1][0],
			p_projection->Points[2][3] + p_projection->Points[2][0],
			p_projection->Points[3][3] + p_projection->Points[3][0]
		);

		l_frustum.Right = PlaneM::build(
			p_projection->Points[0][3] - p_projection->Points[0][0],
			p_projection->Points[1][3] - p_projection->Points[1][0],
			p_projection->Points[2][3] - p_projection->Points[2][0],
			p_projection->Points[3][3] - p_projection->Points[3][0]
		);

		l_frustum.Bottom = PlaneM::build(
			p_projection->Points[0][3] + p_projection->Points[0][1],
			p_projection->Points[1][3] + p_projection->Points[1][1],
			p_projection->Points[2][3] + p_projection->Points[2][1],
			p_projection->Points[3][3] + p_projection->Points[3][1]
		);

		l_frustum.Up = PlaneM::build(
			p_projection->Points[0][3] - p_projection->Points[0][1],
			p_projection->Points[1][3] - p_projection->Points[1][1],
			p_projection->Points[2][3] - p_projection->Points[2][1],
			p_projection->Points[3][3] - p_projection->Points[3][1]
		);

		l_frustum.Near = PlaneM::build(
			p_projection->Points[0][3] + p_projection->Points[0][2],
			p_projection->Points[1][3] + p_projection->Points[1][2],
			p_projection->Points[2][3] + p_projection->Points[2][2],
			p_projection->Points[3][3] + p_projection->Points[3][2]
		);

		l_frustum.Far = PlaneM::build(
			p_projection->Points[0][3] - p_projection->Points[0][2],
			p_projection->Points[1][3] - p_projection->Points[1][2],
			p_projection->Points[2][3] - p_projection->Points[2][2],
			p_projection->Points[3][3] - p_projection->Points[3][2]
		);
#endif
#if 1
		Matrix<4, 4, float> l_projection_inverted;
		MatrixM::inv(p_projection, &l_projection_inverted);

		Vector4<float> l_right_up_far, l_right_up_near, l_right_bottom_far, l_right_bottom_near;
		Vector4<float> l_left_up_far, l_left_up_near, l_left_bottom_far, l_left_bottom_near;

		Vector4<float> tmp_vec4_0 = { 1.0f, 1.0f, 1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_up_far);
		VectorM::mul(&l_right_up_far, 1.0f / l_right_up_far.w, &l_right_up_far);
		// l_right_up_far.z *= -1;

		tmp_vec4_0 = { 1.0f, 1.0f, -1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_up_near);
		VectorM::mul(&l_right_up_near, 1.0f / l_right_up_near.w, &l_right_up_near);
		// l_right_up_near.z *= -1;

		tmp_vec4_0 = { 1.0f, -1.0f, 1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_far);
		VectorM::mul(&l_right_bottom_far, 1.0f / l_right_bottom_far.w, &l_right_bottom_far);
		// l_right_bottom_far.z *= -1;

		tmp_vec4_0 = { 1.0f, -1.0f, -1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_right_bottom_near);
		VectorM::mul(&l_right_bottom_near, 1.0f / l_right_bottom_near.w, &l_right_bottom_near);
		// l_right_bottom_near.z *= -1;


		tmp_vec4_0 = { -1.0f, 1.0f, 1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_up_far);
		VectorM::mul(&l_left_up_far, 1.0f / l_left_up_far.w, &l_left_up_far);
		// l_left_up_far.z *= -1;

		tmp_vec4_0 = { -1.0f, 1.0f, -1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_up_near);
		VectorM::mul(&l_left_up_near, 1.0f / l_left_up_near.w, &l_left_up_near);
		// l_left_up_near.z *= -1;

		tmp_vec4_0 = { -1.0f, -1.0f, 1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_far);
		VectorM::mul(&l_left_bottom_far, 1.0f / l_left_bottom_far.w, &l_left_bottom_far);
		// l_left_bottom_far.z *= -1;

		tmp_vec4_0 = { -1.0f, -1.0f, -1.0f, 1.0f };
		MatrixM::mul(&l_projection_inverted, &tmp_vec4_0, &l_left_bottom_near);
		VectorM::mul(&l_left_bottom_near, 1.0f / l_left_bottom_near.w, &l_left_bottom_near);
		// l_left_bottom_near.z *= -1;


		l_frustum.Left = PlaneM::build(VectorM::cast(&l_left_bottom_near), VectorM::cast(&l_left_bottom_far), VectorM::cast(&l_left_up_near));
		l_frustum.Right = PlaneM::build(VectorM::cast(&l_right_bottom_near), VectorM::cast(&l_right_up_near), VectorM::cast(&l_right_bottom_far));

		l_frustum.Bottom = PlaneM::build(VectorM::cast(&l_left_bottom_near), VectorM::cast(&l_right_bottom_near), VectorM::cast(&l_left_bottom_far));
		l_frustum.Up = PlaneM::build(VectorM::cast(&l_left_up_near), VectorM::cast(&l_left_up_far), VectorM::cast(&l_right_up_near));

		l_frustum.Near = PlaneM::build(VectorM::cast(&l_left_bottom_near), VectorM::cast(&l_left_up_near), VectorM::cast(&l_right_bottom_near));
		l_frustum.Far = PlaneM::build(VectorM::cast(&l_left_bottom_far), VectorM::cast(&l_right_bottom_far), VectorM::cast(&l_left_up_far));

		//Why far and near normals are inverted ?
		VectorM::mul(&l_frustum.Near.Normal, -1.0f, &l_frustum.Near.Normal);
		VectorM::mul(&l_frustum.Far.Normal, -1.0f, &l_frustum.Far.Normal);
		VectorM::mul(&l_frustum.Near.Point, -1.0f, &l_frustum.Near.Point);
		VectorM::mul(&l_frustum.Far.Point, -1.0f, &l_frustum.Far.Point);

#endif
		return l_frustum;
	};
}