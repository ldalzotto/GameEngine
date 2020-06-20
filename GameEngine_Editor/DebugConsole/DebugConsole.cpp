#include "DebugConsole.h"

#include <string>

#include "GameEngineApplicationInterface.h"

#include "MyLog/MyLog.h"
#include "imgui.h"

#include "ECS_Impl/Components/Transform/Transform.h"

#include "IMGuiRender/DrawableWindow.h"
#include "EditWindows/TransformWindow.h"

#include "DebugCommandParser.h"

namespace _GameEngineEditor
{

	void executeDebugCommand(DebugConsole* p_debugConsole, DebugCommand* p_debugCommand);

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
				MYLOG_PUSH(p_debugConsole->GameEngineApplicationInterface->Log, _Log::WARN, (char*)l_return.c_str());
			}
			else
			{
				executeDebugCommand(p_debugConsole, &l_debugCommand);
			}
		}
		ImGui::PopItemWidth();
		ImGui::End();
	};
	
	void executeDebugCommand(DebugConsole* p_debugConsole, DebugCommand* p_debugCommand)
	{
		if (p_debugCommand->Verb == "print")
		{
			MYLOG_PUSH(p_debugConsole->GameEngineApplicationInterface->Log, _Log::WARN, (char*)p_debugCommand->Arguments.at(0).Value.c_str());
		}
		else if (p_debugCommand->Verb == "transform_show")
		{
			if (p_debugCommand->Arguments.size() > 0 && p_debugCommand->Arguments[0].Name == "addr")
			{
				size_t l_val = std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0);
				TransformWindow* l_transformWindow = new TransformWindow();
				l_transformWindow->Transform = reinterpret_cast<_GameEngine::_ECS::Transform*>(l_val);
				p_debugConsole->DrawableWindows->DrawableWindows.push_back(TransformWindow_allocDrawableWindow(&l_transformWindow));
			}
		}
		else if (p_debugCommand->Verb == "transform_attach")
		{
			if (p_debugCommand->Arguments.size() == 2
				&& p_debugCommand->Arguments[0].Name == "parent"
				&& p_debugCommand->Arguments[1].Name == "child")
			{
				_ECS::Transform* l_parent = reinterpret_cast<_GameEngine::_ECS::Transform*>(std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0));
				_ECS::Transform* l_child = reinterpret_cast<_GameEngine::_ECS::Transform*>(std::stoull(p_debugCommand->Arguments[1].Value, nullptr, 0));
				_ECS::Transform_addChild(l_parent, l_child);
			}

		}
		else if (p_debugCommand->Verb == "transform_detach_parent")
		{
			if (p_debugCommand->Arguments.size() == 1 
						&& p_debugCommand->Arguments[0].Name == "addr")
			{
				_ECS::Transform* l_transform = reinterpret_cast<_GameEngine::_ECS::Transform*>(std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0));
				_ECS::Transform_detachParent(l_transform);
			}

		}
	};
}