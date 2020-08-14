#include "DebugConsole.h"

#if 0
#include <string>

#include "GameEngineApplicationInterface.h"

#include "Log/Log.hpp"

#include "imgui.h"

#include "v2/Transform/TransformM.hpp"

#include "ECS_Impl/Components/Transform/TransformComponent.h"

#include "Editor/IMGuiRender/DrawableWindow.h"
#include "Editor/EditWindows/TransformWindow.h"

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
				MYLOG_PUSH(p_debugConsole->GameEngineApplicationInterface->Log, ::_Core::LogLevel::LOG_WARN, (char*)l_return.c_str());
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
			MYLOG_PUSH(p_debugConsole->GameEngineApplicationInterface->Log, ::_Core::LogLevel::LOG_INFO, (char*)p_debugCommand->Arguments.at(0).Value.c_str());
		}
		else if (p_debugCommand->Verb == "transform_show")
		{
			if (p_debugCommand->Arguments.size() > 0 && p_debugCommand->Arguments[0].Name == "addr")
			{
				size_t l_val = std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0);
				TransformWindow* l_transformWindow = new TransformWindow();
				l_transformWindow->Transform = reinterpret_cast<_GameEngine::_ECS::TransformComponent*>(l_val);
				p_debugConsole->DrawableWindows->DrawableWindows.push_back(TransformWindow_allocDrawableWindow(&l_transformWindow));
			}
		}
		else if (p_debugCommand->Verb == "transform_attach")
		{
			if (p_debugCommand->Arguments.size() == 2
				&& p_debugCommand->Arguments[0].Name == "parent"
				&& p_debugCommand->Arguments[1].Name == "child")
			{
				_ECS::TransformComponent* l_parent = reinterpret_cast<_GameEngine::_ECS::TransformComponent*>(std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0));
				_ECS::TransformComponent* l_child = reinterpret_cast<_GameEngine::_ECS::TransformComponent*>(std::stoull(p_debugCommand->Arguments[1].Value, nullptr, 0));
				_MathV2::TransformM::addChild(&l_parent->Transform, &l_child->Transform);
			}

		}
		else if (p_debugCommand->Verb == "transform_detach_parent")
		{
			if (p_debugCommand->Arguments.size() == 1 
						&& p_debugCommand->Arguments[0].Name == "addr")
			{
				_ECS::TransformComponent* l_transform = reinterpret_cast<_GameEngine::_ECS::TransformComponent*>(std::stoull(p_debugCommand->Arguments[0].Value, nullptr, 0));
				_MathV2::TransformM::detachParent(&l_transform->Transform);
			}

		}
	};
}
#endif