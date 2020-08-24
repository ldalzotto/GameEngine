#pragma once


#include "ObjectSelection/EntitySelection.h"

typedef struct GameEngineApplicationInterface_TYP GameEngineApplicationInterface;


typedef struct GameEngineEditor_TYP
{
	GameEngineApplicationInterface* GameEngineApplicationInterface;
	EntitySelection EntitySelection;
}GameEngineEditor, * GameEngineEditor_PTR;

void GameEngineEditor_alloc(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
void GameEngineEditor_free(GameEngineEditor* p_gameEngineEditor, GameEngineApplicationInterface* p_gameEngineApplicationInterface);
void GameEngineEditor_update(GameEngineEditor* p_gameEngineEditor);
