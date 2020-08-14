#pragma once

#if 0
#include "Editor/IMGuiRender/DrawableWindow.h"

namespace _GameEngine::_ECS
{
	struct TransformComponent;
}

using namespace _GameEngine;

namespace _GameEngineEditor
{
	struct TransformWindow
	{
		_ECS::TransformComponent* Transform;
		bool Open;
	};

	DrawableWindow TransformWindow_allocDrawableWindow(TransformWindow** p_transformWindow);
}
#endif