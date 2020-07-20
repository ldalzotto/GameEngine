#pragma once

namespace _GameEngine
{
	namespace _Render { struct RenderInterface; }
	namespace _Input { struct Input; }
	namespace _Physics { struct PhysicsInterface; }
	namespace _ECS { struct ECS; struct Entity; }
}

namespace _GameEngineEditor
{
	struct GameEngineEditor;
}

namespace _GameEngineEditor
{
	struct EntitySelection
	{
		_GameEngine::_ECS::ECS* ECS;
		_GameEngine::_Input::Input* Input;
		_GameEngine::_Render::RenderInterface* RenderInterface;
		_GameEngine::_Physics::PhysicsInterface* PhysicsInterface;
		_GameEngine::_ECS::Entity* SelectedEntity;
	};

	void EntitySelection_build(EntitySelection* p_entitySelection, GameEngineEditor* p_gameEngineEditor);
	void EntitySelection_update(EntitySelection* p_entitySelection);
}