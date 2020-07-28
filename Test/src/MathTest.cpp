#include "Quaternion/Quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
// #include "glm/gtx/rotate_vector.hpp"
#include "Matrix/Matrix.hpp"

int main()
{
	{
		_Math::Quaternion<float> l_zd{};
		l_zd = l_zd.mul(l_zd);
		l_zd.conjugate();


		_Math::Vector3<float> l_direction = _Math::Vector3<float>{ 1.0f, 0.0f, 0.0f }.rotate(_Math::Quaternion<float>::rotateAround(_Math::Vector3<float>{0.0f, 1.0f, 0.0f}, 2.0f));

		glm::vec3 l_rotated = glm::rotate(glm::angleAxis(2.0f, glm::vec3{ 0.0f, 1.0f, 0.0f }), glm::vec3{ 1.0f, 0.0f, 0.0f });
		int zd = 0;

		glm::quat l_left; l_left.x = 0.58f; l_left.y = 0.0f; l_left.z = 0.14f; l_left.w = 0.32f;
		glm::quat l_right; l_right.x = 0.58f; l_right.y = 0.32f; l_right.z = 0.0f; l_right.w = 0.14f;
		glm::quat l_glmRes = l_left * l_right;
		_Math::Quaternion<float> l_meRes = _Math::Quaternion<float>{ 0.58f, 0.0f, 0.14f, 0.32f }.mul(_Math::Quaternion<float>{ 0.58f, 0.32f, 0.0f, 0.14f });

		int lzd = 0;

		_Math::Matrix3x3<float> l_afaefef;
		float l_zdad = l_afaefef.Points[0][2];
	}
	return 0;
}