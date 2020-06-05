#include "GameEngineEditor.h"

#include "GameEngineApplication.h"
#include "EngineSequencers/UpdateSequencer.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_draw(void* p_gameEngineApplication, void* p_delta);

	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplication* p_gameEngineApplication)
	{
		GameEngineEditor* l_gameEngineEditor = new GameEngineEditor();

		l_gameEngineEditor->EngineEditorOperation.Priority = UPDATE_EDITOR_GUI_PRIORITY;
		l_gameEngineEditor->EngineEditorOperation.Closure = l_gameEngineEditor;
		l_gameEngineEditor->EngineEditorOperation.Callback = GameEngineEditor_draw;

		_Utils::SortedSequencer_addOperation(&p_gameEngineApplication->UpdateSequencer.UpdateSequencer, &l_gameEngineEditor->EngineEditorOperation);

		return l_gameEngineEditor;
	};

	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor)
	{
		delete (*p_gameEngineEditor);
		*p_gameEngineEditor = nullptr;
		p_gameEngineEditor = nullptr;
	};

	void GameEngineEditor_draw(void* p_gameEngineApplication, void* p_delta)
	{

	};
}