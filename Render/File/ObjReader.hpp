#pragma once

namespace _RenderV2
{
	struct Mesh;
}

namespace _RenderV2
{
	void ObjReader_loadMesh(const char* p_fileAbsolutePath, Mesh* out_mesh);
}