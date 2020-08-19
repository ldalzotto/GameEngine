#pragma once

extern "C"
{
#include "v2/_interface/TransformC_def.h"
#include "v2/_interface/BoxC_def.h"
#include "v2/_interface/RectC_def.h"
}

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/Vector.hpp"
#include "Renderer/GlobalBuffers/CameraBuffer.hpp"
#include "Raster/Rasterizer.hpp"

namespace _Core
{
	template<typename T>
	struct VectorT;
}

namespace _RenderV2
{
	template<int C, typename T>
	struct Texture;
}

namespace _RenderV2
{
	struct GizmoVertex
	{
		_MathV2::Vector<4, float> WorldPosition;
	};

	using GizmoVertexIndex = size_t;

	struct GizmoLine
	{
		GizmoVertexIndex v1;
		GizmoVertexIndex v2;
		_MathV2::Vector<3, char> Color;
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
		static void render(GizmoRendererInput* p_input, Texture<3, char>* p_to, RECTI_PTR p_to_clipRect, _Core::VectorT<RasterizationStep>* RasterizedPixelsBuffer);

		static void drawLine(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_begin, const _MathV2::Vector3<float>* p_end);
		static void drawLine(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_begin, const _MathV2::Vector3<float>* p_end, const _MathV2::Vector3<char>* p_color);
		static void drawPoint(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_point);
		static void drawPoint(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_point, const _MathV2::Vector3<char>* p_color);
		static void drawBox(GizmoBuffer* p_gizmo, const BOXF_PTR p_box, const MATRIX4F_PTR p_localToWorldMatrix, bool p_withCenter = true, const _MathV2::Vector3<char>* p_color = nullptr);
		static void drawTransform(GizmoBuffer* p_gizmo, TRANSFORM_PTR p_transform);
		static void drawTransform(GizmoBuffer* p_gizmo, const _MathV2::Vector3<float>* p_center, const _MathV2::Vector3<float>* p_right, const _MathV2::Vector3<float>* p_up, const _MathV2::Vector3<float>* p_forward);


	};
}