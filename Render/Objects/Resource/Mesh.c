#include "Mesh.h"

#include "Vertex.h"

void Mesh_Alloc(MESH_PTR p_mesh)
{
	Arr_Alloc_Vertex(&p_mesh->Vertices, 0);
	Arr_Alloc_Polygon_VertexIndex(&p_mesh->Polygons, 0);
};

void Mesh_Free(MESH_PTR p_mesh)
{
	Arr_Free_Vertex(&p_mesh->Vertices);
	Arr_Free_Polygon_VertexIndex(&p_mesh->Polygons);
};