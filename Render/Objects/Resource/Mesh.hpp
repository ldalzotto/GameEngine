#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Vertex.hpp"
#include "Polygon.hpp"

namespace _RenderV2
{
	using VertexIndex = size_t;

	struct Mesh
	{
		_Core::VectorT<Vertex> Vertices;
		_Core::VectorT<Polygon<VertexIndex>> Polygons;
	};
}