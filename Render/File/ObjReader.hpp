#pragma once


extern "C"
{
#include "Objects/Resource/Mesh_def.h"
}

namespace _RenderV2
{
	void ObjReader_loadMesh(const char* p_fileAbsolutePath, MESH_PTR out_mesh);
}