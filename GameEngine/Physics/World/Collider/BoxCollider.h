#pragma once

#include "Math/Box/Box.h"
#include "Math/Matrix/Matrix.h"

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		_Math::Box Box;
		_Math::Matrix4x4f ModelMatrix;
	};
}