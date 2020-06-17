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

	void Quaternionf_copy(Quaternionf* p_source, Quaternionf* p_target);
	bool Quaternionf_equals(Quaternionf* p_left, Quaternionf* p_right);
}