#pragma once

#include "DataStructures/Specifications/ArrayT.hpp"
#include "Vertex.hpp"
#include "Polygon.hpp"

namespace _RenderV2
{
	struct Mesh
	{
		_Core::ArrayT<Vertex> Vertices;
		_Core::ArrayT<Polygon<Vertex*>> Polygons;
	};
}