#pragma once

#include <vector>
#include "glm/glm.hpp"
#include "Render/Mesh/VertexBuffer.h"

#include "Render/Hardware/Device/Device.h"
using namespace _GameEngine::_Render::_Hardware;

namespace _GameEngine::_Render::_Mesh
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
	};

	struct Mesh
	{
		std::vector<Vertex> Vertices;
		VertexBuffer VertexBuffer;
	};

	struct MeshAllocInfo
	{
		std::vector<Vertex>* Vertices;
		_Device::Device* Device;
	};

	void Mesh_alloc(Mesh* p_mesh, MeshAllocInfo* p_meshAllocInfo);
	void Mesh_free(Mesh* p_mesh, _Device::Device* p_device);
}