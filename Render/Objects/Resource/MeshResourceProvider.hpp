#pragma once

#include "DataStructures/Specifications/String.hpp"
#include "Functional/ResourceProvider/ResourceProviderT.hpp"

extern "C"
{
#include "Objects/Resource/Mesh_def.h"
}

namespace _RenderV2
{
	struct MeshResourceKey
	{
		_Core::String MeshPathAbsolute;
	};

	void MeshResourceKey_alloc(MeshResourceKey* p_key, char* p_meshPath);
	void MeshResourceKey_free(MeshResourceKey* p_key);

	size_t MeshResourceKey_getHashCode(MeshResourceKey* p_meshResourceKey);

	struct MeshResource
	{
		MeshResourceKey Key;
		MESH Mesh;
	};

	void MeshResource_allocate(MeshResource* p_meshResource, MeshResourceKey* p_key);
	void MeshResource_free(MeshResource* p_meshResource);

}