#include "Math.h"

#include "glm/gtc/matrix_access.hpp"

namespace _GameEngine::_Utils
{
	glm::mat4x4 Math_TRS(const glm::vec3& p_position, const glm::quat& p_quaternion, const glm::vec3& p_scale)
	{
		glm::mat3x3 l_rotation = glm::mat3x3(p_quaternion);
		return glm::mat4x4(
			glm::vec4(glm::column(l_rotation, 0) * p_scale.x, 0.0f),
			glm::vec4(glm::column(l_rotation, 1) * p_scale.y, 0.0f),
			glm::vec4(glm::column(l_rotation, 2) * p_scale.z, 0.0f),
			glm::vec4(p_position, 0.0f)
		);
	};
}