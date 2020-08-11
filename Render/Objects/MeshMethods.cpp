#include "MeshMethods.hpp"

namespace _RenderV2
{
	void Mesh_alloc(Mesh* p_mesh)
	{
		_Core::VectorT_alloc(&p_mesh->Vertices, 0);
		_Core::VectorT_alloc(&p_mesh->Polygons, 0);
	};
	
	void Mesh_free(Mesh* p_mesh)
	{
		_Core::VectorT_free(&p_mesh->Vertices);
		_Core::VectorT_free(&p_mesh->Polygons);
	};
}