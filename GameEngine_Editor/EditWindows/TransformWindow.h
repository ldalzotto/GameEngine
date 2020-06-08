#pragma once

#include "IMGuiRender/DrawableWindow.h"

namespace _GameEngine::_ECS
{
	struct Transform;
}

using namespace _GameEngine;

namespace _GameEngineEditor
{
	struct TransformWindow
	{
		_ECS::Transform* Transform;
		bool Open;
	};

	DrawableWindow TransformWindow_allocDrawableWindow(TransformWindow** p_transformWindow);
}