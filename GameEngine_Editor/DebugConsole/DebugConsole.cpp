#include "DebugConsole.h"

#include "imgui.h"

namespace _GameEngineEditor
{
	void DebugConsole_draw(DebugConsole* p_debugConsole)
	{
		ImGui::Begin("console");
		ImGui::PushItemWidth(-1);
		ImGui::InputText("", p_debugConsole->CurrentWritingInput, 256);
		ImGui::PopItemWidth();
		ImGui::End();
	};
}