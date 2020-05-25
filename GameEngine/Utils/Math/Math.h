#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace _GameEngine::_Utils
{
	glm::mat4x4 Math_TRS(const glm::vec3& p_position, const glm::quat& p_quaternion, const glm::vec3& p_scale);
}