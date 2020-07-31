#include "v2/Quaternion/QuaternionMath.hpp"
#include "glm/gtx/quaternion.hpp"
#include "v2/Matrix/Matrix.hpp"
#include "Math/Vector/Vector.h"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "Math/Matrix/Matrix.h"
#include "Math/Matrix/MatrixMath.h"

#include "glm/gtx/vector_angle.hpp"

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

		_MathV2::Quaternion<float> l_rightQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{1.0f, 0.0f, 0.0f});
		_MathV2::Quaternion<float> l_upQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{0.0f, 1.0f, 0.0f});
		_MathV2::Quaternion<float> l_forwardQuat = QuaternionM::fromDirection(_MathV2::Vector3<float>{0.0f, 0.0f, 1.0f});

		_MathV2::Quaternion<float> l_rot = QuaternionM::mul(l_forwardQuat, QuaternionM::rotateAround(_MathV2::FORWARD, 0.25f));
		// _MathV2::Matrix3x3<float> l_axises = l_rot.extractAxis();
	// _Core::Qua
		_MathV2::Matrix3x3<float> l_axis = QuaternionM::extractAxis(l_rot);
		glm::mat3x3 l_axisGLM = glm::mat3_cast(*(glm::quat*) & l_rot);
		glm::quat l_castedQuat = glm::quat_cast(l_axisGLM);

		_MathV2::Quaternion<float> l_returnedRot = QuaternionM::fromAxis(l_axis);
		// float l_angle = glm::orientedAngle(glm::normalize(glm::vec3{ 0.0f, 0.681971550f, 0.523248672f }) , glm::normalize(glm::vec3{ 0.0f, 0.706513405f, 0.522750854f }));
	

		_MathV2::Matrix4x4<float> l_trs = _MathV2::MatrixM::buildTRS(_MathV2::Vector3<float>{1.0f, 2.0f, 3.0f}, _MathV2::Quaternion<float>{1.0f, 0.23f, 0.17f, 0.84f}, _MathV2::Vector3<float>{2.0f, 1.0f, 3.0f});

		int zd = 0;
	}
	return 0;
}