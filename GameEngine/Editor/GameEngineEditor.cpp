#include "GameEngineEditor.h"

#include "GameEngineApplicationInterface.h"
#include "Functional/Callback/ObserverT.hpp"

// #include "ECS/ECS.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_update(void* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null);

	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		p_gameEngineEditor->GameEngineApplicationInterface = p_gameEngineApplicationInterface;

		p_gameEngineEditor->OnPreRender = { GameEngineEditor_draw, p_gameEngineEditor };
		_Core::ObserverT_register(p_gameEngineEditor->GameEngineApplicationInterface->PreRender, (_Core::CallbackT<void, GameEngineApplicationInterface>*) & p_gameEngineEditor->OnPreRender);

	};

	void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		// IMGuiRender_free(&(p_gameEngineEditor)->IMGuiRender, p_gameEngineApplicationInterface);
	};

	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null)
	{
		// DebugConsole_draw(&p_gameEngineEditor->DebugConsole);
		DrawableWindows_draw(&p_gameEngineEditor->DrawableWindows);
	};

	void GameEngineEditor_update(void* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		GameEngineEditor* l_gameEngineEditor = (GameEngineEditor*)p_gameEngineEditor;
		// EntitySelection_update(&l_gameEngineEditor->EntitySelection);
	};
}