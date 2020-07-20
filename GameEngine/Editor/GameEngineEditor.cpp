#include "GameEngineEditor.h"

#include "GameEngineApplicationInterface.h"

#include "ECS/ECS.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null);


	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		p_gameEngineEditor->GameEngineApplicationInterface = p_gameEngineApplicationInterface;

		p_gameEngineEditor->OnPreRender = { GameEngineEditor_draw, p_gameEngineEditor };
		_Core::ObserverT_register(p_gameEngineEditor->GameEngineApplicationInterface->PreRender,(_Core::CallbackT<void, GameEngineApplicationInterface>*) & p_gameEngineEditor->OnPreRender);

		IMGuiRender_init(&p_gameEngineEditor->IMGuiRender, p_gameEngineEditor->GameEngineApplicationInterface);

		p_gameEngineEditor->DebugConsole.GameEngineApplicationInterface = p_gameEngineApplicationInterface;
		p_gameEngineEditor->DebugConsole.DrawableWindows = &p_gameEngineEditor->DrawableWindows;
	};

	void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		IMGuiRender_free(&(p_gameEngineEditor)->IMGuiRender, p_gameEngineApplicationInterface);
	};

	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null)
	{
		DebugConsole_draw(&p_gameEngineEditor->DebugConsole);
		DrawableWindows_draw(&p_gameEngineEditor->DrawableWindows);
	};
}