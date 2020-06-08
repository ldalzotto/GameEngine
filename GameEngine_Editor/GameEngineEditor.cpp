#include "GameEngineEditor.h"

#include "GameEngineApplication.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_draw(void* p_gameEngineApplication, void* null);

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplication* p_gameEngineApplication)
	{
		GameEngineEditor* l_gameEngineEditor = new GameEngineEditor();
		l_gameEngineEditor->GameEngineApplication = p_gameEngineApplication;

		l_gameEngineEditor->OnPreRender.Closure = l_gameEngineEditor;
		l_gameEngineEditor->OnPreRender.Callback = GameEngineEditor_draw;
		_Utils::Observer_register(&p_gameEngineApplication->PreRender, &l_gameEngineEditor->OnPreRender);
		
		IMGuiRender_init(&l_gameEngineEditor->IMGuiRender, p_gameEngineApplication);

		l_gameEngineEditor->DebugConsole.DrawableWindows = &l_gameEngineEditor->DrawableWindows;

		return l_gameEngineEditor;
	};

	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor, GameEngineApplication* p_gameEngineApplication)
	{
		IMGuiRender_free(&(*p_gameEngineEditor)->IMGuiRender, p_gameEngineApplication);
		delete (*p_gameEngineEditor);
		*p_gameEngineEditor = nullptr;
		p_gameEngineEditor = nullptr;
	};

	void GameEngineEditor_draw(void* p_gameEngineEditor, void* null)
	{
		GameEngineEditor* l_gameEngineEditor = (GameEngineEditor*)p_gameEngineEditor;
		DebugConsole_draw(&l_gameEngineEditor->DebugConsole);
		DrawableWindows_draw(&l_gameEngineEditor->DrawableWindows);
	};
}