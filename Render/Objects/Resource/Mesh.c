#include "Mesh.h"

#include "v2/_interface/BoxC_alg.h"
#include "Heap/RenderHeap.h"
#include "Vertex.h"

void Mesh_Alloc(Mesh_PTR p_mesh)
{
	Arr_Alloc_VertexHANDLE(&p_mesh->Vertices, 0);
	Arr_Alloc_Polygon_VertexIndex(&p_mesh->Polygons, 0);
};

void Mesh_AllocVertex(Mesh_PTR p_mesh, Vertex_HANDLE_PTR out_vertexHandle)
{
	PoolAllocator_AllocElement_Vertex(&RRenderHeap.VertexAllocator, out_vertexHandle);
	Arr_PushBackRealloc_VertexHANDLE(&p_mesh->Vertices, out_vertexHandle);
};

void Mesh_Free(Mesh_PTR p_mesh)
{
	PoolAllocator_FreeElements(&RRenderHeap.VertexAllocator.allocator, &p_mesh->Vertices);
	Arr_Free(&p_mesh->Vertices.array);
	Arr_Free_Polygon_VertexIndex(&p_mesh->Polygons);
};

void Mesh_BuildBoundingBox(const Mesh_PTR p_mesh, BoxF_PTR p_box)
{
	/*
	#GEN BOX_BUILD_ALROGITHM(
		for (size_t i = 0; i < p_mesh->Vertices.Size; i++) {
			Vector3f_PTR l_point = &RRenderHeap.VertexAllocator.array.Memory[p_mesh->Vertices.Memory[i].Handle].LocalPosition.Vec3; ,
		},
		p_mesh->Vertices.Size
	);
	*/

	Vector3f l_approximateCenter = Vector3f_ZERO;
	{
		for (size_t i = 0; i < p_mesh->Vertices.Size; i++)
		{
			Vector3f_PTR l_point = &RRenderHeap.VertexAllocator.array.Memory[p_mesh->Vertices.Memory[i].Handle].LocalPosition.Vec3;
			Vec_Add_3f_3f(&l_approximateCenter, l_point, &l_approximateCenter);
		}
		Vec_Mul_3f_1f(&l_approximateCenter, 1.0f / p_mesh->Vertices.Size, &l_approximateCenter);
	}
	Vector3f l_min = l_approximateCenter, l_max = l_approximateCenter;
	{
		for (size_t i = 0; i < p_mesh->Vertices.Size; i++)
		{
			Vector3f_PTR l_point = &RRenderHeap.VertexAllocator.array.Memory[p_mesh->Vertices.Memory[i].Handle].LocalPosition.Vec3;
			if (l_point->x <= l_min.x)
			{
				l_min.x = l_point->x;
			}
			if (l_point->y <= l_min.y)
			{
				l_min.y = l_point->y;
			}
			if (l_point->z <= l_min.z)
			{
				l_min.z = l_point->z;
			}
			if (l_point->x >= l_max.x)
			{
				l_max.x = l_point->x;
			}
			if (l_point->y >= l_max.y)
			{
				l_max.y = l_point->y;
			}
			if (l_point->z >= l_max.z)
			{
				l_max.z = l_point->z;
			}
		}
	}
	{
		p_box->Center = Vector3f_ZERO;
		Vec_Add_3f_3f(&p_box->Center, &l_min, &p_box->Center);
		Vec_Add_3f_3f(&p_box->Center, &l_max, &p_box->Center);
		Vec_Mul_3f_1f(&p_box->Center, 0.5f, &p_box->Center);
	}
	Box_Grow_F(p_box, &l_min);
	Box_Grow_F(p_box, &l_max);
};