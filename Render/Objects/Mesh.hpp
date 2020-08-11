#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Vertex.hpp"
#include "Polygon.hpp"

namespace _RenderV2
{
	struct Mesh
	{
		_Core::VectorT<Vertex> Vertices;
		_Core::VectorT<Polygon<Vertex*>> Polygons;
	};
}