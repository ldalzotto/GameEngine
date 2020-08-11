#pragma once

namespace _Core
{
	template<typename T>
	struct InsertorT;
}

namespace _RenderV2
{
	struct Mesh;
}

namespace _RenderV2
{
	void ObjReader_loadMeshes(const char* p_fileAbsolutePath, _Core::InsertorT<Mesh>* out_mesh);
}