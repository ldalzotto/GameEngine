#include "TransformWindow.h"
#include "ECS_Impl/Components/Transform/TransformComponent.h"
#include "imgui.h"
#include "v2/Transform/TransformM.hpp"

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
		_MathV2::Vector3<float> l_localPosition = l_transformWindow->Transform->Transform.LocalPosition;
		if (ImGui::InputFloat3("pos", &l_localPosition.x, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			_MathV2::TransformM::setLocalPosition(&l_transformWindow->Transform->Transform, &l_localPosition);
		}
		_MathV2::Quaternion<float> l_localRotation = l_transformWindow->Transform->Transform.LocalRotation;
		if (ImGui::DragFloat4("rot", &l_localRotation.x, 0.1f, -3.14f, 3.14f))
		{
			_MathV2::TransformM::setLocalRotation(&l_transformWindow->Transform->Transform, &l_localRotation);
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