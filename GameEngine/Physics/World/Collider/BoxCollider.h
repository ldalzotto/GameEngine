#pragma once

namespace _GameEngine::_Math
{
	struct Transform;
	struct Box;
}

namespace _GameEngine::_Physics
{
	struct BoxCollider
	{
		_Math::Transform* Transform;
		_Math::Box* Box;
	};
}