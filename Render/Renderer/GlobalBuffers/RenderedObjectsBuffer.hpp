#pragma once

#include "Objects/RenderedObject.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _RenderV2
{
	struct RenderedObjectsBuffer
	{
		_Core::VectorT<RenderedObject> RenderedObjects;
	};
}