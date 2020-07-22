#pragma once

#include "Render/Materials/Material.h"
#include "ECS_Impl/Components/MeshRenderer/MeshRenderer.h"

using namespace _GameEngine;

extern _Render::MaterialUniqueKey CubeMeshRendererMaterialKey;
extern _ECS::MeshRendererInitInfo CubeMeshRendererInit;

void EntityConfiguration_init();