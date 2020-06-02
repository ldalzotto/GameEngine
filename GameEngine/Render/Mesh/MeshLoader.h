#pragma once

#include <string>
#include <vector>

#include "Render/Mesh/Mesh.h"

namespace _GameEngine::_Render
{

	struct ReadMeshFromFileInfo
	{
		std::string Path;
		std::vector<Vertex>* Vertices;
		std::vector<uint16_t>* Indices;
	};

	void MeshLoader_readMeshFromFile(ReadMeshFromFileInfo* p_readMeshFromFileInfo);
}