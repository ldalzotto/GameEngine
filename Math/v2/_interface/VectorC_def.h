#pragma once

typedef struct VECTOR2F_TYP
{
	union
	{
		float Points[2];
		struct { float x, y; };
	};
} VECTOR2F, * VECTOR2F_PTR;

typedef struct VECTOR3F_TYP
{
	union
	{
		float Points[3];
		struct { float x, y, z; };
		struct { VECTOR2F Vec2; float Vec2_w; };
	};
} VECTOR3F, *VECTOR3F_PTR;

typedef struct VECTOR4F_TYP
{
	union
	{
		float Points[4];
		struct { float x, y, z, w; };
		struct { VECTOR3F Vec3; float Vec3_w; };
	};
} VECTOR4F, * VECTOR4F_PTR;

typedef struct VECTOR2D_TYP
{
	union
	{
		struct { double x, y; };
		double Points[2];
	};
} VECTOR2D, * VECTOR2D_PTR;

typedef struct VECTOR2I_TYP
{
	union
	{
		struct { int x, y; };
		double Points[2];
	};
} VECTOR2I, * VECTOR2I_PTR;