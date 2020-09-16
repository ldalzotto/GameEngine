#pragma once

#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"

typedef struct DirectionalLight_TYP
{
	Vector3f Direction;
	float Intensity;
	Color3f Color;
}DirectionalLight, * DirectionalLight_PTR;

typedef struct AmbientLight_TYP
{
	Color3f Color;
}AmbientLight, * AmbientLight_PTR;

typedef struct RenderLights_TYP
{
	DirectionalLight DirectionalLight;
	AmbientLight AmbientLight;
}RenderLights, * RenderLights_PTR;