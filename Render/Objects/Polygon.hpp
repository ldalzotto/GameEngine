#pragma once

namespace _RenderV2
{
	struct Vertex;
}

namespace _RenderV2
{
	template <typename T>
	struct Polygon
	{
		T v1;
		T v2;
		T v3;
	};
}