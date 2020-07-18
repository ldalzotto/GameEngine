#include "GameEngineEditor.h"

#include "GameEngineApplicationInterface.h"

#include "ECS/ECS.h"
#include "ECS/Systems/MeshRendererBoundGizmoSystem.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null);
	void gameEngineEditor_systemInitialization(GameEngineEditor* p_gameEngineEditor);


	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		GameEngineEditor* l_gameEngineEditor = new GameEngineEditor();
		l_gameEngineEditor->GameEngineApplicationInterface = p_gameEngineApplicationInterface;

		l_gameEngineEditor->OnPreRender = { GameEngineEditor_draw, l_gameEngineEditor };
		_Core::ObserverT_register(l_gameEngineEditor->GameEngineApplicationInterface->PreRender,(_Core::CallbackT<void, GameEngineApplicationInterface>*) &l_gameEngineEditor->OnPreRender);

		IMGuiRender_init(&l_gameEngineEditor->IMGuiRender, l_gameEngineEditor->GameEngineApplicationInterface);

		l_gameEngineEditor->DebugConsole.GameEngineApplicationInterface = p_gameEngineApplicationInterface;
		l_gameEngineEditor->DebugConsole.DrawableWindows = &l_gameEngineEditor->DrawableWindows;

		gameEngineEditor_systemInitialization(l_gameEngineEditor);

		return l_gameEngineEditor;
	};

	void GameEngineEditor_free(GameEngineEditor** p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
	{
		IMGuiRender_free(&(*p_gameEngineEditor)->IMGuiRender, p_gameEngineApplicationInterface);
		delete (*p_gameEngineEditor);
		*p_gameEngineEditor = nullptr;
		p_gameEngineEditor = nullptr;
	};

	void GameEngineEditor_draw(GameEngineEditor* p_gameEngineEditor, void* null)
	{
		DebugConsole_draw(&p_gameEngineEditor->DebugConsole);
		DrawableWindows_draw(&p_gameEngineEditor->DrawableWindows);
	};

	void gameEngineEditor_systemInitialization(GameEngineEditor* p_gameEngineEditor)
	{
		/*
		_ECS::SystemV2AllocInfo	l_systemAllocInfo = {};
		MeshRendererBoundGizmoSystem_init(&l_systemAllocInfo, p_gameEngineEditor->GameEngineApplicationInterface->ECS, p_gameEngineEditor->GameEngineApplicationInterface->RenderInterface);
		_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, p_gameEngineEditor->GameEngineApplicationInterface->UpdateSequencer);
		*/
	};
}