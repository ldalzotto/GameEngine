#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "v2/Vector/Vector.hpp"

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

	struct Gizmo
	{
		static void render(GizmoBuffer* p_gizmoBuffer);
	};
}