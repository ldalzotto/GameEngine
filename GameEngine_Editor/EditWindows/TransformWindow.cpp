#include "TransformWindow.h"
#include "ECS_Impl/Components/Transform/Transform.h"
#include "imgui.h"

namespace _GameEngineEditor
{
	void TransformWindow_draw(DrawableWindow* p_transformWindow);
	void TransformWindow_onClose(DrawableWindow* p_transformWindow);

	DrawableWindow TransformWindow_allocDrawableWindow(TransformWindow** p_transformWindow)
	{
		DrawableWindow l_drawableWindow{};
		l_drawableWindow.Window = *p_transformWindow;
		l_drawableWindow.Draw = TransformWindow_draw;
		l_drawableWindow.OnClosed = TransformWindow_onClose;
		l_drawableWindow.IsOpen = true;
		return l_drawableWindow;
	};

	void TransformWindow_draw(DrawableWindow* p_transformWindow)
	{
		TransformWindow* l_transformWindow = (TransformWindow*)p_transformWindow->Window;
		ImGui::Begin("Transform", &p_transformWindow->IsOpen);
		_Math::Vector3f l_localPosition = l_transformWindow->Transform->LocalPosition;
		if (ImGui::InputFloat3("pos", &l_localPosition.x, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			_ECS::Transform_setLocalPosition(l_transformWindow->Transform, l_localPosition);
		}
		_Math::Quaternionf l_localRotation = l_transformWindow->Transform->LocalRotation;
		if (ImGui::DragFloat4("rot", &l_localRotation.x, 0.1f, -3.14f, 3.14f))
		{
			_ECS::Transform_setLocalRotation(l_transformWindow->Transform, l_localRotation);
		}
		ImGui::End();
	};
	
	void TransformWindow_onClose(DrawableWindow* p_transformWindow)
	{
		TransformWindow* l_transformWindow = (TransformWindow*)p_transformWindow->Window;
		delete l_transformWindow;
		p_transformWindow->Window = nullptr;
	};
}