#include "ObjReader.hpp"

#include "Read/File/File.hpp"

extern "C"
{
#include "Objects/Resource/Vertex.h"
#include "Objects/Resource/Mesh.h"
}
#include "Algorithm/String/StringAlgorithm.hpp"

namespace _RenderV2
{
	void ObjReader_loadMesh(const char* p_fileAbsolutePath, MESH_PTR out_mesh)
	{
		_Core::FileStream l_fs = _Core::FileStream_open(p_fileAbsolutePath);
		_Core::FileLineIterator l_it = _Core::FileStream_allocLineIterator(&l_fs);
		{
			_Core::StringSlice l_spaceSlice = { " ", 0, 1 };
			_Core::StringSlice l_slashSlice = { "/", 0, 1 };
			_Core::StringSlice l_oHeader = { "o", 0, 1 };
			_Core::StringSlice l_vHeader = { "v", 0, 1 };
			_Core::StringSlice l_fHeader = { "f", 0, 1 };

			bool l_meshProcessed = false;
			while (_Core::FileLineIterator_moveNext(&l_it))
			{
				size_t l_dataBegin_index;
				_Core::StringSlice l_lineSlice = _Core::String_buildSlice(&l_it.Line);
				_Core::String_find_v2(&l_lineSlice, &l_spaceSlice, &l_dataBegin_index);

				_Core::StringSlice l_lineHeaderSlice = { l_it.Line.Memory, 0, l_dataBegin_index };
				_Core::StringSlice l_lineWithoutHeader = { l_it.Line.Memory + l_dataBegin_index + 1, 0, l_it.Line.Size - (l_dataBegin_index + 1) };

				if (_Core::String_equals(&l_lineHeaderSlice, &l_oHeader))
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
				else if (_Core::String_equals(&l_lineHeaderSlice, &l_vHeader))
				{
					if (out_mesh)
					{
						_Core::VectorT<_Core::String> l_verticesPositions;
						_Core::VectorT_alloc(&l_verticesPositions, 3);
						{

							_Core::String_split(&l_lineWithoutHeader, &l_spaceSlice, &l_verticesPositions);
							if (l_verticesPositions.Size > 0)
							{
								VERTEX l_insertedVertex = {};
								Arr_PushBackRealloc_Vertex(&out_mesh->Vertices, &l_insertedVertex);
								VERTEX_PTR l_vertex = &out_mesh->Vertices.Memory[out_mesh->Vertices.Size - 1];
								l_vertex->LocalPosition.x = (float)atof(_Core::VectorT_at(&l_verticesPositions, 0)->Memory);
								l_vertex->LocalPosition.y = (float)atof(_Core::VectorT_at(&l_verticesPositions, 1)->Memory);
								l_vertex->LocalPosition.z = (float)atof(_Core::VectorT_at(&l_verticesPositions, 2)->Memory);
							}

						}
						_Core::String_freeIteratorOfStrings(&_Core::VectorT_buildIterator(&l_verticesPositions));
						_Core::VectorT_free(&l_verticesPositions);
					}
				}
				else if (_Core::String_equals(&l_lineHeaderSlice, &l_fHeader))
				{
					if (out_mesh)
					{
						_Core::VectorT<_Core::String> l_polyFaces; _Core::VectorT_alloc(&l_polyFaces, 3);
						{
							_Core::String_split(&l_lineWithoutHeader, &l_spaceSlice, &l_polyFaces);
							if (l_polyFaces.Size > 0)
							{
								POLYGON_VERTEXINDEX l_insertedPoly = {};
								Arr_PushBackRealloc_Polygon_VertexIndex(&out_mesh->Polygons, &l_insertedPoly);
								POLYGON_VERTEXINDEX_PTR l_polygon = &out_mesh->Polygons.Memory[out_mesh->Polygons.Size - 1];

								_Core::VectorT<_Core::String> l_polygVertexIndices; _Core::VectorT_alloc(&l_polygVertexIndices, 3);

								for (size_t i = 0; i < l_polyFaces.Size; i++)
								{
									_Core::VectorT_clear(&l_polygVertexIndices);
									_Core::String_split(&_Core::String_buildSlice(_Core::VectorT_at(&l_polyFaces, i)), &l_slashSlice, &l_polygVertexIndices);
									if (l_polygVertexIndices.Size > 0)
									{
										VertexIndex* l_vertexIndex = nullptr;
										switch (i)
										{
										case 0:
											l_vertexIndex = &l_polygon->v1;
											break;
										case 1:
											l_vertexIndex = &l_polygon->v2;
											break;
										case 2:
											l_vertexIndex = &l_polygon->v3;
											break;
										}

										if (l_vertexIndex)
										{
											*l_vertexIndex = atoi(_Core::VectorT_at(&l_polygVertexIndices, 0)->Memory) - 1;
										}

									}
									_Core::String_freeIteratorOfStrings(&_Core::VectorT_buildIterator(&l_polygVertexIndices));
									_Core::VectorT_clear(&l_polygVertexIndices);
								}
								_Core::VectorT_free(&l_polygVertexIndices);
							}
						}
						_Core::String_freeIteratorOfStrings(&_Core::VectorT_buildIterator(&l_polyFaces));
						_Core::VectorT_free(&l_polyFaces);
					}
				}
			}
		}
		_Core::FileLineIterator_free(&l_it);
		_Core::FileStream_close(&l_fs);
	};
}