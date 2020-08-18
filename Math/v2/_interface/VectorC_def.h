#pragma once
typedef struct VECTOR2F_TYP
{
	union
	{
		struct { float x, y; };
		float Points[2];
	};
} VECTOR2F, * VECTOR2F_PTR;

typedef struct VECTOR3F_TYP
{
	union
	{
		struct { float x, y, z; };
		float Points[3];
	};
} VECTOR3F, *VECTOR3F_PTR;

typedef struct VECTOR4F_TYP
{
	union
	{
		struct { float x, y, z, w; };
		float Points[4];
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
