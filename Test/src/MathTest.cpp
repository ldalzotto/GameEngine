#include "v2/Quaternion/QuaternionMath.hpp"
#include "glm/gtx/quaternion.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "Math/Vector/Vector.h"
#include "v2/Vector/VectorMath.hpp"
#include "Math/Quaternion/Quaternion.h"
#include "Math/Quaternion/QuaternionMath.h"

using namespace _GameEngine;
using namespace _MathV2;

int main()
{
	{
		_MathV2::Quaternion<float> l_zd{};
		l_zd = QuaternionM::mul(l_zd, l_zd);
		QuaternionM::conjugate(l_zd);


		_MathV2::Vector3<float> l_direction = VectorM::rotate(Vector3<float>{ 1.0f, 0.0f, 0.0f }, QuaternionM::rotateAround(_MathV2::Vector3<float>{0.0f, 1.0f, 0.0f}, 2.0f));

		glm::vec3 l_rotated = glm::rotate(glm::angleAxis(2.0f, glm::vec3{ 0.0f, 1.0f, 0.0f }), glm::vec3{ 1.0f, 0.0f, 0.0f });

		glm::quat l_left; l_left.x = 0.58f; l_left.y = 0.0f; l_left.z = 0.14f; l_left.w = 0.32f;
		glm::quat l_right; l_right.x = 0.58f; l_right.y = 0.32f; l_right.z = 0.0f; l_right.w = 0.14f;
		glm::quat l_glmRes = l_left * l_right;
		_MathV2::Quaternion<float> l_meRes = QuaternionM::mul(Quaternion<float>{0.58f, 0.0f, 0.14f, 0.32f}, Quaternion<float>{0.58f, 0.32f, 0.0f, 0.14f });

		_MathV2::Matrix3x3<float> l_afaefef;
		float l_zdad = l_afaefef.Points[0][2];

		_MathV2::Quaternion<float> l_rightQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{1.0f, 0.0f, 0.0f});
		_MathV2::Quaternion<float> l_upQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{0.0f, 1.0f, 0.0f});
		_MathV2::Quaternion<float> l_forwardQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{0.0f, 0.0f, 1.0f});

		_MathV2::Quaternion<float> l_rot = QuaternionM::mul(l_forwardQuat, QuaternionM::rotateAround(_MathV2::FORWARD, 0.25f));
		// _MathV2::Matrix3x3<float> l_axises = l_rot.extractAxis();
	// _Core::Qua
		_MathV2::Matrix3x3<float> l_axis = QuaternionM::extractAxis(l_rot);
		glm::mat3x3 l_axisGLM = glm::mat3_cast(*(glm::quat*) & l_rot);
		glm::quat l_castedQuat = glm::quat_cast(l_axisGLM);

		{
			_Math::Quaternionf l_rotAsQuat = *(_Math::Quaternionf*)(&l_rot);
			_Math::Vector3f l_right, l_up, l_forward;
			_Math::Quaternion_extractAxis(&l_rotAsQuat, &l_right, &l_up, &l_forward);
			int zd = 0;

		}
		_MathV2::Quaternion<float> l_returnedRot = QuaternionM::fromAxis(l_axis);

	}
	return 0;
}