#pragma once

#include "VectorC_def.h"
#include "DataStructures/ARRAY_def.h"

typedef struct Color3f_TYP
{
	union
	{
		Vector3f Vec;
		struct { float r; float g; float b; };
	};
	
}Color3f, * Color3f_PTR;

typedef struct Array_Color3f_TYP
{
	ARRAY_TYPE_DEFINITION(Color3f)
}Array_Color3f, * Array_Color3f_PTR;

typedef struct Color3c_TYP
{
	union
	{
		Vector3c Vec;
		struct { unsigned char r; unsigned char g; unsigned char b; };
	};

}Color3c, * Color3c_PTR;

typedef struct Array_Color3c_TYP
{
	ARRAY_TYPE_DEFINITION(Color3c)
}Array_Color3c, * Array_Color3c_PTR;

typedef struct Color4c_TYP
{
	union
	{
		Vector4c Vec;
		struct { unsigned char r; unsigned char g; unsigned char b; unsigned char a; };
	};

}Color4c, * Color4c_PTR;

