#include "Quaternion/Quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
// #include "glm/gtx/rotate_vector.hpp"

int main()
{
	{
		_Math::Quaternion<float> l_zd{};
		l_zd = _Math::Quaternion_mul(l_zd, l_zd);
		_Math::Quaternion_conjugate(l_zd);


		_Math::Vector3 l_direction = _Math::Vector_rotate(_Math::Vector3<float> { 1.0f, 0.0f, 0.0f }, _Math::Quaternion_rotateAround(_Math::Vector3<float>{1.0f, 0.0f, 0.0f}, 2.0f));


		//	 glm::quat{ l_deltaRotation.x, l_deltaRotation.y, l_deltaRotation.z, l_deltaRotation.w } *l_direction;
		glm::vec3 l_rotated = glm::rotate(glm::angleAxis(2.0f, glm::vec3{ 1.0f, 0.0f, 0.0f }), glm::vec3{ 1.0f, 0.0f, 0.0f });
		int zd = 0;

		/*
		glm::quat l_left; l_left.x = 0.58f; l_left.y = 0.0f; l_left.z = 0.14f; l_left.w = 0.32f;
		glm::quat l_right; l_right.x = 0.58f; l_right.y = 0.32f; l_right.z = 0.0f; l_right.w = 0.14f;
		glm::quat l_glmRes = l_left * l_right;
		_Math::Quaternion<float> l_meRes = _Math::Quaternion_mul(_Math::Quaternion<float>{ 0.58f, 0.0f, 0.14f, 0.32f }, _Math::Quaternion<float>{ 0.58f, 0.32f, 0.0f, 0.14f });

		int lzd = 0;
		*/
	}
	return 0;
}