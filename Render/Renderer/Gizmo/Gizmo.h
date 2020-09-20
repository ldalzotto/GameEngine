#pragma once


#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "v2/_interface/ColorC_def.h"
#include "v2/_interface/WindowSize_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "DataStructures/ARRAY_def.h"


typedef struct GizmoVertex_TYP
{
	Vector4f WorldPosition;
}GizmoVertex, * GizmoVertex_PTR;

typedef size_t GizmoVertexIndex;

typedef struct GizmoLine_TYP
{
	GizmoVertexIndex v1;
	GizmoVertexIndex v2;
	Color3c Color;
} GizmoLine;

typedef struct ARRAY_GIZMOVertex_TYP
{
	ARRAY_TYPE_DEFINITION(GizmoVertex)
} ARRAY_GIZMOVertex, * ARRAY_GIZMOVertex_PTR;

typedef struct ARRAY_GIZMOLINE_TYP
{
	ARRAY_TYPE_DEFINITION(GizmoLine)
} ARRAY_GIZMOLINE, * ARRAY_GIZMOLINE_PTR;

typedef struct GizmoBuffer_TYP
{
	ARRAY_GIZMOVertex Vertices;
	ARRAY_GIZMOLINE Lines;
} GizmoBuffer;

void GizmoBuffer_alloc(GizmoBuffer* p_buffer);
void GizmoBuffer_clear(GizmoBuffer* p_buffer);
void GizmoBuffer_free(GizmoBuffer* p_buffer);

typedef struct GizmoRendererInput_TYP
{
	GizmoBuffer* Buffer;
	CAMERABUFFER_PTR CameraBuffer;
	WindowSize WindowSize;
} GizmoRendererInput;

void Gizmo_Render(GizmoRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect);
void Gizmo_DrawLine(GizmoBuffer* p_gizmo, const Vector3f_PTR p_begin, const Vector3f_PTR p_end);
void Gizmo_DrawLine_Color(GizmoBuffer* p_gizmo, const Vector3f_PTR p_begin, const Vector3f_PTR p_end, const Color3c_PTR p_color);
void Gizmo_DrawPoint(GizmoBuffer* p_gizmo, const Vector3f_PTR p_point);
void Gizmo_DrawPoint_Color(GizmoBuffer* p_gizmo, const Vector3f_PTR p_point, const Color3c_PTR p_color);
void Gizmo_DrawBox(GizmoBuffer* p_gizmo, const BoxF_PTR p_box, const Matrix4f_PTR p_localToWorldMatrix, bool p_withCenter, const Color3c_PTR p_color);
void Gizmo_DrawTransform(GizmoBuffer* p_gizmo, Transform_PTR p_transform);
void Gizmo_DrawTransform_Axis(GizmoBuffer* p_gizmo, const Vector3f_PTR p_center, const Vector3f_PTR p_right, const Vector3f_PTR p_up, const Vector3f_PTR p_forward);
