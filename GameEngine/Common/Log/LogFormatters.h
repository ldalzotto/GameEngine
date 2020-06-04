#pragma once

#include "glm/glm.hpp"

#include <string>

std::string LOG_MAT4(const glm::mat4x4& mat)
{
	return "\n [ " + std::to_string(mat[0][0]) + " , " + std::to_string(mat[0][1]) + " , " + std::to_string(mat[0][2]) + " , " + std::to_string(mat[0][3]) + " ], \n "
		+ "[ " + std::to_string(mat[1][0]) + " , " + std::to_string(mat[1][1]) + " , " + std::to_string(mat[1][2]) + " , " + std::to_string(mat[1][3]) + " ], \n "
		+ "[ " + std::to_string(mat[2][0]) + " , " + std::to_string(mat[2][1]) + " , " + std::to_string(mat[2][2]) + " , " + std::to_string(mat[2][3]) + " ], \n "
		+ "[ " + std::to_string(mat[3][0]) + " , " + std::to_string(mat[3][1]) + " , " + std::to_string(mat[3][2]) + " , " + std::to_string(mat[3][3]) + " ]";
}