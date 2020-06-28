#pragma once

namespace _GameEngine::_Math
{
	struct Vector3f;
}

namespace _GameEngine::_Math
{

	struct Quaternionf
	{
		float x;
		float y;
		float z;
		float w;
	};

	Quaternionf Quaternionf_identity();
}