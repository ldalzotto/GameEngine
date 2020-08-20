#include "MeshResourceProvider.hpp"

extern "C"
{
#include "Objects/Resource/Mesh.h"
#include "File/ObjReader.h"
}
#include "Functional/Hash/Hash.hpp"

namespace _RenderV2
{
	size_t MeshResourceKey_getHashCode(MeshResourceKey* p_meshResourceKey)
	{
		return _Core::Hash_string(p_meshResourceKey->MeshPathAbsolute.Memory);
	};

	void MeshResourceKey_alloc(MeshResourceKey* p_key, char* p_meshPath)
	{
		_Core::String_alloc(&p_key->MeshPathAbsolute, 0); _Core::String_append(&p_key->MeshPathAbsolute, p_meshPath);
	};
	
	void MeshResourceKey_free(MeshResourceKey* p_key)
	{
		_Core::String_free(&p_key->MeshPathAbsolute);
	};

	void MeshResource_allocate(MeshResource* p_meshResource, MeshResourceKey* p_key)
	{
		ObjReader_loadMesh(p_key->MeshPathAbsolute.Memory, &p_meshResource->Mesh);
		p_meshResource->Key = *p_key;
		p_key->MeshPathAbsolute.Memory = nullptr; //We steal memory allocated from mesh key.
	};
	
	void MeshResource_free(MeshResource* p_meshResource)
	{
		Mesh_Free(&p_meshResource->Mesh);
		_Core::String_free(&p_meshResource->Key.MeshPathAbsolute);
	};
}