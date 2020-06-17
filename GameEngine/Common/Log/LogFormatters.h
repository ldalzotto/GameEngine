#pragma once

#include "Math/Matrix/Matrix.h"
#include "Math/Vector/Vector.h"
#include <string>

std::string LOG_MAT4(const _GameEngine::_Math::Matrix4x4f& mat)
{
	return "\n [ " + std::to_string(mat._00) + " , " + std::to_string(mat._10) + " , " + std::to_string(mat._20) + " , " + std::to_string(mat._30) + " ], \n "
		    + "[ " + std::to_string(mat._01) + " , " + std::to_string(mat._11) + " , " + std::to_string(mat._21) + " , " + std::to_string(mat._31) + " ], \n "
		    + "[ " + std::to_string(mat._02) + " , " + std::to_string(mat._12) + " , " + std::to_string(mat._22) + " , " + std::to_string(mat._32) + " ], \n "
		    + "[ " + std::to_string(mat._03) + " , " + std::to_string(mat._13) + " , " + std::to_string(mat._23) + " , " + std::to_string(mat._33) + " ]";
}

std::string LOG_VEC3(const _GameEngine::_Math::Vector3f& vec)
{
	return "\n [ " + std::to_string(vec.x) + " , " + std::to_string(vec.y) + " , " + std::to_string(vec.z) + " ]";
}