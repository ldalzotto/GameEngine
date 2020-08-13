#include "WireframeRenderer.hpp"

#include <cmath>

#include "Constants.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Box/Box.hpp"
#include "v2/Sphere/Sphere.hpp"
#include "v2/Segment/SegmentV2Math.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Intersection/Intersection.h"
#include "v2/Rect/Rect.hpp"

#include "Objects/RenderedObject.hpp"
#include "Objects/Resource/PolygonMethods.hpp"
#include "Objects/Resource/Vertex.hpp"
#include "Objects/Resource/Mesh.hpp"
#include "Objects/Texture/TextureM.hpp"

#include "Raster/Rasterizer.hpp"

#include <string.h>

#include "v2/Frustum/FrustumMath.hpp"

using namespace _MathV2;

namespace _RenderV2
{
	void DrawLineClipped(
		Vector<4, float>* p_begin, Vector<4, float>* p_end,
		_Core::VectorT<_MathV2::Vector<2, int>>* in_out_rasterizedPixelsBuffer, _Core::VectorT<bool>* in_rasterizerBufferV2, _MathV2::Rect<int>* p_clipRect)
	{
		_Core::VectorT_clear(in_out_rasterizedPixelsBuffer);

		// We -1 width and height because we use indices maximum indices (and not pixel count)
		Rasterizer::line_v3_clipped((Vector2<float>*) p_begin, (Vector2<float>*) p_end, in_out_rasterizedPixelsBuffer, p_clipRect);
		for (size_t j = 0; j < in_out_rasterizedPixelsBuffer->Size; j++)
		{
			Vector2<int>* l_pixel = &in_out_rasterizedPixelsBuffer->Memory[j];
			*(bool*)(((char*)in_rasterizerBufferV2->Memory) + TextureM::getElementOffset(l_pixel->x, l_pixel->y, p_clipRect->Max.x + 1, sizeof(bool))) = true;
		}

	}
	void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture<3, char>* p_to, _MathV2::Rect<int>* p_to_clipRect, WireframeRenderer_Memory* p_memory)
	{
		WireframeRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);

		Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2;

		{
			Frustum l_cameraFrustum = FrustumM::extractFrustumFromProjection(p_input->ProjectionMatrix);

			for (size_t i = 0; i < p_input->RenderableObjects->Size; i++)
			{
				RenderedObject* l_renderableObject = &p_input->RenderableObjects->Memory[i];

				{

					Box* l_renderBoxBound_localSpace = &l_renderableObject->MeshBoundingBox;
					Sphere l_renderBox_asSphere_cameraSpace;

					Matrix4x4<float> l_object_to_camera;
					MatrixM::mul(p_input->ViewMatrix, l_renderableObject->ModelMatrix, &l_object_to_camera);

					{
						MatrixM::mul(&l_object_to_camera, &VectorM::cast(&l_renderBoxBound_localSpace->Center, 1.0f), &tmp_vec4_0);
						l_renderBox_asSphere_cameraSpace.Center = *VectorM::cast(&tmp_vec4_0);
					}

					{
						SegmentV2<4, float> l_box_extend_localSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, VectorM::cast(&l_renderBoxBound_localSpace->Extend, 1.0f) };
						SegmentV2<4, float> l_box_extend_worldSpace;
						SegmentM::mul(&l_box_extend_localSpace, l_renderableObject->ModelMatrix, &l_box_extend_worldSpace);
						Vector<4, float> l_box_extend_worldSpace_vec; SegmentM::toVector(&l_box_extend_worldSpace, &l_box_extend_worldSpace_vec);
						l_renderBox_asSphere_cameraSpace.Radius = VectorM::length(VectorM::cast(&l_box_extend_worldSpace_vec));
					}

					if (Intersection_Contains_Frustum_Sphere(&l_cameraFrustum, &l_renderBox_asSphere_cameraSpace))
					{
						// Push polygons
						for (size_t j = 0; j < l_renderableObject->Mesh.Polygons.Size; j++)
						{
							Polygon<VertexIndex>* l_polygon = &l_renderableObject->Mesh.Polygons.Memory[j];

							PolygonPipelineV2 l_polygonPipeline{};
							l_polygonPipeline.RenderedObject = l_renderableObject;
							l_polygonPipeline.PolygonIndex = j;

							l_polygonPipeline.TransformedPolygon = {
								VectorM::cast(&l_renderableObject->Mesh.Vertices.Memory[l_polygon->v1].LocalPosition, 1.0f),
								VectorM::cast(&l_renderableObject->Mesh.Vertices.Memory[l_polygon->v2].LocalPosition, 1.0f),
								VectorM::cast(&l_renderableObject->Mesh.Vertices.Memory[l_polygon->v3].LocalPosition, 1.0f)
							};
							_Core::VectorT_pushBack(&p_memory->PolygonPipelines, &l_polygonPipeline);
						}

					}
				}
			}

			for (size_t i = 0; i < p_memory->PolygonPipelines.Size; i++)
			{
				PolygonPipelineV2* l_polygonPipeline = &p_memory->PolygonPipelines.Memory[i];

				// Local to world
				{
					MatrixM::mul(l_polygonPipeline->RenderedObject->ModelMatrix, &l_polygonPipeline->TransformedPolygon.v1, &tmp_vec4_0);
					MatrixM::mul(l_polygonPipeline->RenderedObject->ModelMatrix, &l_polygonPipeline->TransformedPolygon.v2, &tmp_vec4_1);
					MatrixM::mul(l_polygonPipeline->RenderedObject->ModelMatrix, &l_polygonPipeline->TransformedPolygon.v3, &tmp_vec4_2);

					l_polygonPipeline->TransformedPolygon.v1 = tmp_vec4_0;
					l_polygonPipeline->TransformedPolygon.v2 = tmp_vec4_1;
					l_polygonPipeline->TransformedPolygon.v3 = tmp_vec4_2;
				}

				// Backface culling
				{
					Vector3<float> l_cameraToPolygon = *VectorM::cast(VectorM::min(&l_polygonPipeline->TransformedPolygon.v1, p_input->CameraWorldPosition, &tmp_vec4_0));
					Vector3<float> l_u = *VectorM::cast(VectorM::min(&l_polygonPipeline->TransformedPolygon.v1, &l_polygonPipeline->TransformedPolygon.v2, &tmp_vec4_0));
					Vector3<float> l_v = *VectorM::cast(VectorM::min(&l_polygonPipeline->TransformedPolygon.v1, &l_polygonPipeline->TransformedPolygon.v3, &tmp_vec4_0));
					Vector3<float> l_n;
					VectorM::cross(&l_u, &l_v, &l_n);

					if (VectorM::dot(&l_cameraToPolygon, &l_n) > FLOAT_TOLERANCE)
					{
						continue;
					};
				}

				// World to camera
				{
					MatrixM::mul(p_input->ViewMatrix, &l_polygonPipeline->TransformedPolygon.v1, &tmp_vec4_0);
					MatrixM::mul(p_input->ViewMatrix, &l_polygonPipeline->TransformedPolygon.v2, &tmp_vec4_1);
					MatrixM::mul(p_input->ViewMatrix, &l_polygonPipeline->TransformedPolygon.v3, &tmp_vec4_2);

					l_polygonPipeline->CameraSpacePolygon.v1 = tmp_vec4_0;
					l_polygonPipeline->CameraSpacePolygon.v2 = tmp_vec4_1;
					l_polygonPipeline->CameraSpacePolygon.v3 = tmp_vec4_2;
				}

				Polygon<Vector4<float>> l_targetPolygon;

				// Camera to clip
				{

					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_0, &l_targetPolygon.v1);
					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_1, &l_targetPolygon.v2);
					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_2, &l_targetPolygon.v3);

					VectorM::mul(&l_targetPolygon.v1, 1 / l_targetPolygon.v1.w, &l_targetPolygon.v1);
					VectorM::mul(&l_targetPolygon.v2, 1 / l_targetPolygon.v2.w, &l_targetPolygon.v2);
					VectorM::mul(&l_targetPolygon.v3, 1 / l_targetPolygon.v3.w, &l_targetPolygon.v3);

					//TODO -> Near clipping ? (https://gamedev.stackexchange.com/questions/22295/how-should-i-handling-clipping-vertices-that-are-closer-to-the-eye-than-the-near)
				}

				// To pixel
				{
					float l_screenDepth = l_targetPolygon.v1.z;
					l_targetPolygon.v1.z = 1.0f; l_targetPolygon.v1.w = 1.0f;
					l_polygonPipeline->TransformedPolygon.v1 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v1, &tmp_vec4_0));
					l_polygonPipeline->TransformedPolygon.v1.z = l_screenDepth;

					l_screenDepth = l_targetPolygon.v2.z;
					l_targetPolygon.v2.z = 1.0f; l_targetPolygon.v2.w = 1.0f;
					l_polygonPipeline->TransformedPolygon.v2 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v2, &tmp_vec4_0));
					l_polygonPipeline->TransformedPolygon.v2.z = l_screenDepth;

					l_screenDepth = l_targetPolygon.v3.z;
					l_targetPolygon.v3.z = 1.0f; l_targetPolygon.v3.w = 1.0f;
					l_polygonPipeline->TransformedPolygon.v3 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v3, &tmp_vec4_0));
					l_polygonPipeline->TransformedPolygon.v3.z = l_screenDepth;
				}

				// Rasterize
				{
					DrawLineClipped(&l_polygonPipeline->TransformedPolygon.v1, &l_polygonPipeline->TransformedPolygon.v2, &p_memory->RasterizedPixelsBuffer, &p_memory->RasterizerBufferV2, p_to_clipRect );
					DrawLineClipped(&l_polygonPipeline->TransformedPolygon.v2, &l_polygonPipeline->TransformedPolygon.v3, &p_memory->RasterizedPixelsBuffer, &p_memory->RasterizerBufferV2, p_to_clipRect);
					DrawLineClipped(&l_polygonPipeline->TransformedPolygon.v3, &l_polygonPipeline->TransformedPolygon.v1, &p_memory->RasterizedPixelsBuffer, &p_memory->RasterizerBufferV2, p_to_clipRect);
				}
			}

			for (int w = 0; w < p_to->Width; w++)
			{
				for (int h = 0; h < p_to->Height; h++)
				{

					bool* l_write = (bool*)((char*)p_memory->RasterizerBufferV2.Memory + TextureM::getElementOffset(w, h, p_to->Width, sizeof(bool)));
					if (*l_write)
					{
						_MathV2::Vector3<char>* l_pixel = (_MathV2::Vector3<char>*) ((char*)p_to->Pixels.Memory + TextureM::getElementOffset(w, h, p_to->Width, p_to->Pixels.ElementSize));
						*l_pixel = { 0,0,0 };
					}
				}
			}
		}
	};


	void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory)
	{
		_Core::VectorT_alloc(&p_memory->PolygonPipelines, 0);
		_Core::VectorT_alloc(&p_memory->RasterizedPixelsBuffer, 0);
		_Core::VectorT_alloc(&p_memory->RasterizerBufferV2, 0);
	};
	void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height)
	{
		_Core::VectorT_clear(&p_memory->PolygonPipelines);
		_Core::VectorT_clear(&p_memory->RasterizedPixelsBuffer);
		_Core::VectorT_resize(&p_memory->RasterizedPixelsBuffer, p_width > height ? p_width * 2 : height * 2);
		_Core::VectorT_resize(&p_memory->RasterizerBufferV2, p_width * height);
		_Core::VectorT_clear(&p_memory->RasterizerBufferV2);
		memset(p_memory->RasterizerBufferV2.Memory, 0, p_memory->RasterizerBufferV2.ElementSize * p_memory->RasterizerBufferV2.Capacity);

	};
	void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory)
	{
		_Core::VectorT_free(&p_memory->PolygonPipelines);
		_Core::VectorT_free(&p_memory->RasterizedPixelsBuffer);
		_Core::VectorT_free(&p_memory->RasterizerBufferV2);
	};
}