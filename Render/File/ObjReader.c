#include "ObjReader.h"

#include "Read/File/File.h"
#include "DataStructures/String.h"
#include "v2/_interface/VectorStructuresC.h"
#include "Objects/Resource/Vertex.h"
#include "Objects/Resource/Mesh.h"
#include "Objects/Resource/Polygon.h"

#include "Heap/RenderHeap.h"

void ObjReader_loadMesh(const char* p_fileAbsolutePath, Mesh_PTR out_mesh)
{
	FileStream l_fs = FileStream_open(p_fileAbsolutePath, FILESTREAM_MODE_READ);
	FileLineIterator l_it = FileStream_allocLineIterator(&l_fs);
	{
		StringSLICE l_spaceSlice = { " ", 0, 1 };
		StringSLICE l_slashSlice = { "/", 0, 1 };
		StringSLICE l_oHeader = { "o", 0, 1 };
		StringSLICE l_vHeader = { "v", 0, 1 };
		StringSLICE l_vtHeader = { "vt", 0, 2 };
		StringSLICE l_fHeader = { "f", 0, 1 };

		Array_Vector2f l_uvs; Arr_Alloc_Vector2f(&l_uvs, 0);

		bool l_meshProcessed = false;
		while (FileLineIterator_moveNext(&l_it))
		{
			size_t l_dataBegin_index;
			String* l_lineStringC = &l_it.Line;
			StringSLICE l_lineSlice = { l_lineStringC->Memory, 0, strlen(l_lineStringC->Memory) };
			String_Find(&l_lineSlice, &l_spaceSlice, &l_dataBegin_index);

			StringSLICE l_lineHeaderSlice = { l_it.Line.Memory, 0, l_dataBegin_index };
			StringSLICE l_lineWithoutHeader = { l_it.Line.Memory + l_dataBegin_index + 1, 0, l_it.Line.Size - (l_dataBegin_index + 1) };

			if (String_Equals(&l_lineHeaderSlice, &l_oHeader))
			{
				if (l_meshProcessed)
				{
					return;
				}
				if (out_mesh)
				{
					Mesh_Alloc(out_mesh);
					l_meshProcessed = true;
				}
			}
			else if (String_Equals(&l_lineHeaderSlice, &l_vHeader))
			{
				if (out_mesh)
				{
					Array_String l_verticesPositions;
					Arr_Alloc_String(&l_verticesPositions, 3);
					{

						String_Split(&l_lineWithoutHeader, &l_spaceSlice, &l_verticesPositions);
						if (l_verticesPositions.Size > 0)
						{
							Vertex_HANDLE l_insertedVertex;
							Mesh_AllocVertex(out_mesh, &l_insertedVertex);
							Vertex_PTR l_vertex = &RRenderHeap.VertexAllocator.array.Memory[l_insertedVertex.Handle];
							l_vertex->LocalPosition = (Vector4f){ (float)atof(l_verticesPositions.Memory[0].Memory), (float)atof(l_verticesPositions.Memory[1].Memory) , (float)atof(l_verticesPositions.Memory[2].Memory), 1.0f };
						}

					}
					for (size_t i = 0; i < l_verticesPositions.Size; i++)
					{
						String_Free(&l_verticesPositions.Memory[i]);
					}
					Arr_Free_String(&l_verticesPositions);
				}
			}
			else if (String_Equals(&l_lineHeaderSlice, &l_vtHeader))
			{
				if (out_mesh)
				{
					Array_String l_uvPositions;
					Arr_Alloc_String(&l_uvPositions, 2);
					{
						String_Split(&l_lineWithoutHeader, &l_spaceSlice, &l_uvPositions);
						if (l_uvPositions.Size > 0)
						{
							UV l_uv;
							l_uv.x = (float)atof(l_uvPositions.Memory[0].Memory);
							l_uv.y = (float)atof(l_uvPositions.Memory[1].Memory);
							Arr_PushBackRealloc_Vector2f(&l_uvs, &l_uv);
						}
					}
					Arr_Free_String(&l_uvPositions);
				}
			}
			else if (String_Equals(&l_lineHeaderSlice, &l_fHeader))
			{
				if (out_mesh)
				{
					Array_String l_polyFaces; Arr_Alloc_String(&l_polyFaces, 3);
					{
						String_Split(&l_lineWithoutHeader, &l_spaceSlice, &l_polyFaces);
						if (l_polyFaces.Size > 0)
						{
							Polygon_VertexIndex_HANDLE l_insertedPolyHandle;
							PoolAllocator_AllocElement_Polygon(&RRenderHeap.PolygonAllocator, &l_insertedPolyHandle);
							Arr_PushBackRealloc_Polygon_VertexIndex_HANDLE(&out_mesh->Polygons, &l_insertedPolyHandle);
							Polygon_VertexIndex_PTR l_polygon = &RRenderHeap.PolygonAllocator.array.Memory[l_insertedPolyHandle.Handle];

							PolygonSizeT l_polygonUV_index;

							Array_String l_polygVertexIndices; Arr_Alloc_String(&l_polygVertexIndices, 3);

							for (size_t i = 0; i < l_polyFaces.Size; i++)
							{

								Arr_Clear_String(&l_polygVertexIndices);
								String* l_polyFaceString = &l_polyFaces.Memory[i];
								StringSLICE l_polyFaceSlice = { l_polyFaceString->Memory, 0, strlen(l_polyFaceString->Memory) };
								String_Split(&l_polyFaceSlice, &l_slashSlice, &l_polygVertexIndices);
								if (l_polygVertexIndices.Size > 0)
								{
									VertexIndex* l_vertexIndex = NULL;
									size_t* l_uvIndex = NULL;

									switch (i)
									{
									case 0:
										l_vertexIndex = &l_polygon->v1;
										l_uvIndex = &l_polygonUV_index.v1;
										break;
									case 1:
										l_vertexIndex = &l_polygon->v2;
										l_uvIndex = &l_polygonUV_index.v2;
										break;
									case 2:
										l_vertexIndex = &l_polygon->v3;
										l_uvIndex = &l_polygonUV_index.v3;
										break;
									}

									if (l_vertexIndex)
									{
										*l_vertexIndex = atoi(l_polygVertexIndices.Memory[0].Memory) - 1;
									}
									if (l_uvIndex)
									{
										*l_uvIndex = atoi(l_polygVertexIndices.Memory[1].Memory) - 1;
									}
								}
								for (size_t i = 0; i < l_polygVertexIndices.Size; i++)
								{
									String_Free(&l_polygVertexIndices.Memory[i]);
								}
								Arr_Clear_String(&l_polygVertexIndices);
							}

							//Allocating the uv polygons
							Polgyon_UV_HANDLE l_uvPolygonHandle;
							Mesh_AllocPolygonUV(out_mesh, &l_uvPolygonHandle);
							*&RRenderHeap.PolygonUVAllocator.array.Memory[l_uvPolygonHandle.Handle] = 
								(Polygon_UV){ 
								.v1 = l_uvs.Memory[l_polygonUV_index.v1],
								.v2 = l_uvs.Memory[l_polygonUV_index.v2], 
								.v3 = l_uvs.Memory[l_polygonUV_index.v3] };
							

							Arr_Free_String(&l_polygVertexIndices);
						}
					}
					for (size_t i = 0; i < l_polyFaces.Size; i++)
					{
						String_Free(&l_polyFaces.Memory[i]);
					}
					Arr_Free_String(&l_polyFaces);
				}
			}
		}

		// Handling UVs
		Arr_Free(&l_uvs.array);
	}
	FileLineIterator_free(&l_it);
	FileStream_close(&l_fs);
};