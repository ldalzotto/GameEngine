#include "GameEngineEditor.h"

#include "GameEngineApplicationInterface.h"

void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
{
	p_gameEngineEditor->GameEngineApplicationInterface = p_gameEngineApplicationInterface;

	EntitySelection_build(&p_gameEngineEditor->EntitySelection, p_gameEngineEditor);
};

void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface)
{
	// IMGuiRender_free(&(p_gameEngineEditor)->IMGuiRender, p_gameEngineApplicationInterface);
};

void GameEngineEditor_update(GameEngineEditor* p_gameEngineEditor)
{
	EntitySelection_update(&p_gameEngineEditor->EntitySelection);
};
