#include "GameEngineEditor.h"

#include "GameEngineApplicationInterface.h"
#include "EngineSequencers/EngineSequencers.h"

#include "ECS/ECS.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_update(void* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null);

	void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		p_gameEngineEditor->GameEngineApplicationInterface = p_gameEngineApplicationInterface;

		p_gameEngineEditor->OnPreRender = { GameEngineEditor_draw, p_gameEngineEditor };
		_Core::ObserverT_register(p_gameEngineEditor->GameEngineApplicationInterface->PreRender, (_Core::CallbackT<void, GameEngineApplicationInterface>*) & p_gameEngineEditor->OnPreRender);

		IMGuiRender_init(&p_gameEngineEditor->IMGuiRender, p_gameEngineEditor->GameEngineApplicationInterface);

		p_gameEngineEditor->DebugConsole.GameEngineApplicationInterface = p_gameEngineApplicationInterface;
		p_gameEngineEditor->DebugConsole.DrawableWindows = &p_gameEngineEditor->DrawableWindows;

		EntitySelection_build(&p_gameEngineEditor->EntitySelection, p_gameEngineEditor);

		_Core::SortedSequencerOperationT<GameEngineApplicationInterface> l_gameEngineEditorUpdate{};
		l_gameEngineEditorUpdate.OperationCallback = { GameEngineEditor_update, p_gameEngineEditor };

		_Core::VectorT<_Core::SortedSequencerPriority> l_beforePritority; _Core::VectorT_alloc(&l_beforePritority, 1);
		_Core::VectorT<_Core::SortedSequencerPriority> l_afterPriotiry; _Core::VectorT_alloc(&l_afterPriotiry, 1);
		{
			_Core::VectorT_pushBack(&l_beforePritority, _GameEngine::UPDATE_PUSH_TO_RENDER_PRIORITY);
			_Core::VectorT_pushBack(&l_afterPriotiry,  _GameEngine::EDITOR_PRIORITY);
			l_gameEngineEditorUpdate.Priority = _Core::SortedSequencer_calculatePriority(&l_beforePritority, &l_afterPriotiry);
		}
		_Core::VectorT_free(&l_beforePritority);
		_Core::VectorT_free(&l_afterPriotiry);

		_Core::SortedSequencerT_addOperation(&p_gameEngineApplicationInterface->UpdateSequencer->UpdateSequencer, &l_gameEngineEditorUpdate);
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

	void GameEngineEditor_update(void* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		GameEngineEditor* l_gameEngineEditor = (GameEngineEditor*)p_gameEngineEditor;
		EntitySelection_update(&l_gameEngineEditor->EntitySelection);
	};
}