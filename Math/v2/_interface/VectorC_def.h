#pragma once

typedef struct Vector2f_TYP
{
	union
	{
		float Points[2];
		struct { float x, y; };
	};
} Vector2f, * Vector2f_PTR;

typedef struct Vector3f_TYP
{
	union
	{
		float Points[3];
		struct { float x, y, z; };
		struct { Vector2f Vec2; float Vec2_w; };
	};
} Vector3f, *Vector3f_PTR;

typedef struct Vector4f_TYP
{
	union
	{
		float Points[4];
		struct { float x, y, z, w; };
		struct { Vector3f Vec3; float Vec3_w; };
	};
} Vector4f, * Vector4f_PTR;

typedef struct Vector2d_TYP
{
	union
	{
		struct { double x, y; };
		double Points[2];
	};
} Vector2d, * Vector2d_PTR;

typedef struct Vector2i_TYP
{
	union
	{
		struct { int x, y; };
		int Points[2];
	};
} Vector2i, * Vector2i_PTR;

typedef struct Vector3c_TYP
{
	union
	{
		char Points[3];
		struct { char x, y, z; };
	};
} Vector3c, * Vector3c_PTR;