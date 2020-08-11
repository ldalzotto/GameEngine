#include "ObjReader.hpp"

#include "Read/File/File.hpp"
#include "Objects/Mesh.hpp"

#include "DataStructures/Specifications/ArrayT.hpp"
#include "Algorithm/String/StringAlgorithm.hpp"
#include "Objects/MeshMethods.hpp"

namespace _RenderV2
{
	void ObjReader_loadMeshes(const char* p_fileAbsolutePath, _Core::InsertorT<Mesh>* out_mesh)
	{
		_Core::FileStream l_fs = _Core::FileStream_open(p_fileAbsolutePath);
		_Core::FileLineIterator l_it = _Core::FileStream_allocLineIterator(&l_fs);
		{
			_Core::StringSlice l_spaceSlice = { " ", 0, 1 };
			_Core::StringSlice l_slashSlice = { "/", 0, 1 };
			_Core::StringSlice l_oHeader = { "o", 0, 1 };
			_Core::StringSlice l_vHeader = { "v", 0, 1 };
			_Core::StringSlice l_fHeader = { "f", 0, 1 };

			Mesh* l_currentMesh = nullptr;
			size_t l_vertexOffset = 0; // The .obj file doesn't keep adding vertex indices from previous model. 
			while (_Core::FileLineIterator_moveNext(&l_it))
			{
				size_t l_dataBegin_index;
				_Core::StringSlice l_lineSlice = _Core::String_buildSlice(&l_it.Line);
				_Core::String_find_v2(&l_lineSlice, &l_spaceSlice, &l_dataBegin_index);

				_Core::StringSlice l_lineHeaderSlice = { l_it.Line.Memory, 0, l_dataBegin_index };
				_Core::StringSlice l_lineWithoutHeader = { l_it.Line.Memory + l_dataBegin_index + 1, 0, l_it.Line.Size - (l_dataBegin_index + 1) };

				if (_Core::String_equals(&l_lineHeaderSlice, &l_oHeader))
				{
					Mesh l_mesh{};
					if (l_currentMesh)
					{
						l_vertexOffset += l_currentMesh->Vertices.Size;
					}
					l_currentMesh = _Core::InsertorT_pushBack(out_mesh, &l_mesh);
					Mesh_alloc(l_currentMesh);
				}
				else if (_Core::String_equals(&l_lineHeaderSlice, &l_vHeader))
				{
					if (l_currentMesh)
					{
						_Core::VectorT<_Core::String> l_verticesPositions;
						_Core::VectorT_alloc(&l_verticesPositions, 3);
						{

							_Core::String_split(&l_lineWithoutHeader, &l_spaceSlice, &l_verticesPositions);
							if (l_verticesPositions.Size > 0)
							{
								Vertex l_insertedVertex = {};
								Vertex* l_vertex = _Core::VectorT_pushBack(&l_currentMesh->Vertices, &l_insertedVertex);
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
					if (l_currentMesh)
					{
						_Core::VectorT<_Core::String> l_polyFaces; _Core::VectorT_alloc(&l_polyFaces, 3);
						{
							_Core::String_split(&l_lineWithoutHeader, &l_spaceSlice, &l_polyFaces);
							if (l_polyFaces.Size > 0)
							{
								Polygon<Vertex*> l_insertedPoly = {};
								Polygon<Vertex*>* l_polygon = _Core::VectorT_pushBack(&l_currentMesh->Polygons, &l_insertedPoly);
								_Core::VectorT<_Core::String> l_polygVertexIndices; _Core::VectorT_alloc(&l_polygVertexIndices, 3);

								for (size_t i = 0; i < l_polyFaces.Size; i++)
								{
									_Core::VectorT_clear(&l_polygVertexIndices);
									_Core::String_split(&_Core::String_buildSlice(_Core::VectorT_at(&l_polyFaces, i)), &l_slashSlice, &l_polygVertexIndices);
									if (l_polygVertexIndices.Size > 0)
									{
										Vertex** l_vertex = nullptr;
										switch (i)
										{
										case 0:
											l_vertex = &l_polygon->v1;
											break;
										case 1:
											l_vertex = &l_polygon->v2;
											break;
										case 2:
											l_vertex = &l_polygon->v3;
											break;
										}

										if (l_vertex)
										{
											int l_index = atoi(_Core::VectorT_at(&l_polygVertexIndices, 0)->Memory) - l_vertexOffset - 1;
											*l_vertex = _Core::VectorT_at(&l_currentMesh->Vertices, l_index);
										}

									}
								}

								_Core::String_freeIteratorOfStrings(&_Core::VectorT_buildIterator(&l_polygVertexIndices));
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