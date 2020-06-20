#include "GameEngineEditor.h"

#include "GameEngineApplication.h"

#include "Render/Render.h"
#include "ECS/ECS.h"
#include "ECS_Impl/Systems/SystemV2Factory.h"
#include "ECS/Systems/MeshRendererBoundGizmoSystem.h"

namespace _GameEngineEditor
{
	void GameEngineEditor_draw(void* p_gameEngineApplication, void* null);
	void gameEngineEditor_systemInitialization(GameEngineEditor* p_gameEngineEditor);


	GameEngineEditor* GameEngineEditor_alloc(GameEngineApplication* p_gameEngineApplication)
	{
		GameEngineEditor* l_gameEngineEditor = new GameEngineEditor();
		l_gameEngineEditor->GameEngineApplication = p_gameEngineApplication;

		l_gameEngineEditor->OnPreRender.Closure = l_gameEngineEditor;
		l_gameEngineEditor->OnPreRender.Callback = GameEngineEditor_draw;
		_Utils::Observer_register(&p_gameEngineApplication->PreRender, &l_gameEngineEditor->OnPreRender);

		IMGuiRender_init(&l_gameEngineEditor->IMGuiRender, p_gameEngineApplication);

		l_gameEngineEditor->DebugConsole.DrawableWindows = &l_gameEngineEditor->DrawableWindows;

		gameEngineEditor_systemInitialization(l_gameEngineEditor);

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

	void gameEngineEditor_systemInitialization(GameEngineEditor* p_gameEngineEditor)
	{
		_ECS::ECS* l_ecs = p_gameEngineEditor->GameEngineApplication->ECS;
		_Render::RenderInterface* l_renderInterface = &p_gameEngineEditor->GameEngineApplication->Render->RenderInterface;

		_ECS::SystemV2AllocInfo	l_systemAllocInfo = {};
		MeshRendererBoundGizmoSystem_init(&l_systemAllocInfo, l_ecs, l_renderInterface);
		_ECS_Impl::SystemV2Factory_allocSystemV2(&l_systemAllocInfo, &p_gameEngineEditor->GameEngineApplication->UpdateSequencer);
	};
}