#pragma once

extern "C"
{
#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/RectC_def.h"
#include "v2/_interface/VectorC_def.h"
#include "Objects/Texture/Texture_def.h"
#include "Raster/Rasterizer_def.h"
}

#include "DataStructures/Specifications/VectorT.hpp"
#include "Renderer/GlobalBuffers/CameraBuffer.hpp"

namespace _Core
{
	template<typename T>
	struct VectorT;
}

namespace _RenderV2
{
	struct GizmoVertex
	{
		VECTOR4F WorldPosition;
	};

	using GizmoVertexIndex = size_t;

	struct GizmoLine
	{
		GizmoVertexIndex v1;
		GizmoVertexIndex v2;
		VECTOR3C Color;
	};

	struct GizmoBuffer
	{
		_Core::VectorT<GizmoVertex> Vertices;
		_Core::VectorT<GizmoLine> Lines;
	};

	void GizmoBuffer_alloc(GizmoBuffer* p_buffer);
	void GizmoBuffer_clear(GizmoBuffer* p_buffer);
	void GizmoBuffer_free(GizmoBuffer* p_buffer);

	struct GizmoRendererInput
	{
		GizmoBuffer* Buffer;
		CameraBuffer* CameraBuffer;
		MATRIX4F_PTR GraphicsAPIToScreeMatrix;
	};

	struct Gizmo
	{
		static void render(GizmoRendererInput* p_input, TEXTURE3C_PTR p_to, RECTI_PTR p_to_clipRect, ARRAY_RASTERISATIONSTEP_PTR RasterizedPixelsBuffer);

		static void drawLine(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end);
		static void drawLine(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_begin, const VECTOR3F_PTR p_end, const VECTOR3C_PTR p_color);
		static void drawPoint(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point);
		static void drawPoint(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_point, const VECTOR3C_PTR p_color);
		static void drawBox(GizmoBuffer* p_gizmo, const BOXF_PTR p_box, const MATRIX4F_PTR p_localToWorldMatrix, bool p_withCenter = true, const VECTOR3C_PTR p_color = nullptr);
		static void drawTransform(GizmoBuffer* p_gizmo, TRANSFORM_PTR p_transform);
		static void drawTransform(GizmoBuffer* p_gizmo, const VECTOR3F_PTR p_center, const VECTOR3F_PTR p_right, const VECTOR3F_PTR p_up, const VECTOR3F_PTR p_forward);


	};
}