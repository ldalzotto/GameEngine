#include "DebugConsole.h"

#include <string>
#include "Log/Log.h"
#include "imgui.h"

#include "DebugCommandParser.h"

namespace _GameEngineEditor
{

	void executeDebugCommand(DebugCommand* p_debugCommand);

	void DebugConsole_draw(DebugConsole* p_debugConsole)
	{
		ImGui::Begin("console");
		ImGui::PushItemWidth(-1);
		if (ImGui::InputText("", p_debugConsole->CurrentWritingInput, 256, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			std::string l_consoleInput = std::string(p_debugConsole->CurrentWritingInput);
			DebugCommand l_debugCommand{};

			std::string l_return;
			DebugCommand_handleError(DebugCommand_parse(&l_debugCommand, &l_consoleInput), &l_return);
			if (!l_return.empty())
			{
				_GameEngine::_Log::LogInstance->CoreLogger->warn(l_return);
			}
			else
			{
				executeDebugCommand(&l_debugCommand);
			}
		}
		ImGui::PopItemWidth();
		ImGui::End();
	};
	
	void executeDebugCommand(DebugCommand* p_debugCommand)
	{
		if (p_debugCommand->Verb == "print")
		{
			_GameEngine::_Log::LogInstance->CoreLogger->warn(p_debugCommand->Arguments.at(0).Value);
		}
	};
}