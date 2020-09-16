#pragma once

#include "VectorC_def.h"

typedef struct Color3f_TYP
{
	union
	{
		Vector3f Vec;
		struct { float r; float g; float b; };
	};
	
}Color3f, * Color3f_PTR;


typedef struct Color3c_TYP
{
	union
	{
		Vector3c Vec;
		struct { char r; char g; char b; };
	};

}Color3c, * Color3c_PTR;