#pragma once


#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Raster/Rasterizer_def.h"
#include "Renderer/GlobalBuffers/CameraBuffer.h"
#include "DataStructures/ARRAY_def.h"


typedef struct GizmoVertex_TYP
{
	VECTOR4F WorldPosition;
}GizmoVertex, * GizmoVertex_PTR;

typedef size_t GizmoVertexIndex;

typedef struct GizmoLine_TYP
{
	GizmoVertexIndex v1;
	GizmoVertexIndex v2;
	VECTOR3C Color;
} GizmoLine;

typedef struct ARRAY_GIZMOVERTEX_TYP
{
	ARRAY_TYPE_DEFINITION(GizmoVertex)
} ARRAY_GIZMOVERTEX, * ARRAY_GIZMOVERTEX_PTR;

typedef struct ARRAY_GIZMOLINE_TYP
{
	ARRAY_TYPE_DEFINITION(GizmoLine)
} ARRAY_GIZMOLINE, * ARRAY_GIZMOLINE_PTR;

typedef struct GizmoBuffer_TYP
{
	ARRAY_GIZMOVERTEX Vertices;
	ARRAY_GIZMOLINE Lines;
} GizmoBuffer;

void GizmoBuffer_alloc(GizmoBuffer* p_buffer);
void GizmoBuffer_clear(GizmoBuffer* p_buffer);
void GizmoBuffer_free(GizmoBuffer* p_buffer);

typedef struct GizmoRendererInput_TYP
{
	GizmoBuffer* Buffer;
	CAMERABUFFER_PTR CameraBuffer;
	MATRIX4F_PTR GraphicsAPIToScreeMatrix;
} GizmoRendererInput;

void Gizmo_Render(GizmoRendererInput* p_input, TEXTURE3C_PTR p_to, RECTI_PTR p_to_clipRect, ARRAY_RASTERISATIONSTEP_PTR RasterizedPixelsBuffer);
void Gizmo_DrawLine(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end);
void Gizmo_DrawLine_Color(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end, const VECTOR3C_PTR p_color);
void Gizmo_DrawPoint(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point);
void Gizmo_DrawPoint_Color(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point, const VECTOR3C_PTR p_color);
void Gizmo_DrawBox(GizmoBuffer* p_gizmo, const BOXF_PTR p_box, const MATRIX4F_PTR p_localToWorldMatrix, bool p_withCenter, const VECTOR3C_PTR p_color);
void Gizmo_DrawTransform(GizmoBuffer* p_gizmo, TRANSFORM_PTR p_transform);
void Gizmo_DrawTransform_Axis(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_center, const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward);
