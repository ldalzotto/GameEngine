#include "WireframeRenderer.hpp"

#include <cmath>

#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Box/Box.hpp"
#include "v2/Sphere/Sphere.hpp"
#include "v2/Segment/SegmentV2Math.hpp"
#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"
#include "v2/Intersection/Intersection.h"

#include "Objects/Polygon.hpp"
#include "Objects/Vertex.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Texture/TextureM.hpp"

#include "Raster/Rasterizer.hpp"

#include "v2/Frustum/FrustumMath.hpp"

using namespace _MathV2;

namespace _RenderV2
{

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to, WireframeRenderMemory* p_memory)
	{

		WireframeRenderMemory_clear(p_memory);

		_Core::VectorT<RenderableObject>* l_renderedObjects = p_input->RenderableObjects;

		Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2;

		{
			{
				_Core::VectorIteratorT<RenderableObject> l_renderedObjects_it = _Core::VectorT_buildIterator(l_renderedObjects);
				while (_Core::VectorIteratorT_moveNext(&l_renderedObjects_it))
				{
					RenderableObjectPipeline l_renderableObjectPieline = { false, l_renderedObjects_it.Current };
					_Core::VectorT_pushBack(&p_memory->RenderableObjectsPipeline, &l_renderableObjectPieline);
				}
			}

			if (p_input->RendererConfiguration.ObjectCullEnabled)
			{
				//Object culling - Done by creating a bouding sphere from the world porjected mesh bounding box (max value of extend).
//the, projecting to camera space and checking if min/max for every axis are contained in the camera frustum
				{
					/*
						We cannot exectue the object culling in homogeneous clip space, because a point cannot be outside of the unit cube.
						What we have to do :
							- Extract 6 planes from camera frustum.
							- Project the model bounding box to camera space.
							- Check if there is at least a min/max values around the main axis are inside.
					*/

					Frustum l_cameraFrustum = FrustumM::extractFrustumFromProjection(p_input->ProjectionMatrix);

					_Core::VectorIteratorT<RenderableObjectPipeline> l_renderedObjectsPipeline_it = _Core::VectorT_buildIterator(&p_memory->RenderableObjectsPipeline);
					while (_Core::VectorIteratorT_moveNext(&l_renderedObjectsPipeline_it))
					{
						Box* l_renderBoxBound_localSpace = l_renderedObjectsPipeline_it.Current->RenderableObject->MeshBoundingBox;
						Sphere l_renderBox_asSphere_cameraSpace;

						Matrix4x4<float> l_object_to_camera;
						MatrixM::mul(p_input->ViewMatrix, l_renderedObjectsPipeline_it.Current->RenderableObject->ModelMatrix, &l_object_to_camera);

						{
							MatrixM::mul(&l_object_to_camera, &VectorM::cast(&l_renderBoxBound_localSpace->Center, 1.0f), &tmp_vec4_0);
							l_renderBox_asSphere_cameraSpace.Center = *VectorM::cast(&tmp_vec4_0);
						}

						{
							SegmentV2<4, float> l_box_extend_localSpace = { {0.0f, 0.0f, 0.0f, 1.0f}, VectorM::cast(&l_renderBoxBound_localSpace->Extend, 1.0f) };
							SegmentV2<4, float> l_box_extend_worldSpace;
							SegmentM::mul(&l_box_extend_localSpace, l_renderedObjectsPipeline_it.Current->RenderableObject->ModelMatrix, &l_box_extend_worldSpace);
							Vector<4, float> l_box_extend_worldSpace_vec; SegmentM::toVector(&l_box_extend_worldSpace, &l_box_extend_worldSpace_vec);
							l_renderBox_asSphere_cameraSpace.Radius = VectorM::length(VectorM::cast(&l_box_extend_worldSpace_vec));
						}

						l_renderedObjectsPipeline_it.Current->IsCulled = !Intersection_Contains_Frustum_Sphere(&l_cameraFrustum, &l_renderBox_asSphere_cameraSpace);
					}

				}
			}
		}

		// Creating polygon list
		{
			_Core::VectorIteratorT<RenderableObjectPipeline> l_renderedObjects_it = _Core::VectorT_buildIterator(&p_memory->RenderableObjectsPipeline);
			while (_Core::VectorIteratorT_moveNext(&l_renderedObjects_it))
			{
				if (!l_renderedObjects_it.Current->IsCulled)
				{
					_Core::VectorIteratorT<Polygon<Vertex*>> l_meshPolygons_it = _Core::VectorT_buildIterator(&l_renderedObjects_it.Current->RenderableObject->Mesh->Polygons);
					while (_Core::VectorIteratorT_moveNext(&l_meshPolygons_it))
					{
						PolygonPipeline l_polygon{};
						l_polygon.ModelMatrix = l_renderedObjects_it.Current->RenderableObject->ModelMatrix;
						l_polygon.TransformedPolygon = {
							VectorM::cast(&l_meshPolygons_it.Current->v1->LocalPosition, 1.0f),
							VectorM::cast(&l_meshPolygons_it.Current->v2->LocalPosition, 1.0f),
							VectorM::cast(&l_meshPolygons_it.Current->v3->LocalPosition, 1.0f)
						};
						_Core::VectorT_pushBack(&p_memory->PolygonPipeline, &l_polygon);
					}
				}
			}
		}


		{
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&p_memory->PolygonPipeline);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				PolygonPipeline* l_pipelinePolygon = l_polygonsIt.Current;

				// Local to world
				MatrixM::mul(l_pipelinePolygon->ModelMatrix, &l_pipelinePolygon->TransformedPolygon.v1, &tmp_vec4_0);
				MatrixM::mul(l_pipelinePolygon->ModelMatrix, &l_pipelinePolygon->TransformedPolygon.v2, &tmp_vec4_1);
				MatrixM::mul(l_pipelinePolygon->ModelMatrix, &l_pipelinePolygon->TransformedPolygon.v3, &tmp_vec4_2);

				l_pipelinePolygon->TransformedPolygon.v1 = tmp_vec4_0;
				l_pipelinePolygon->TransformedPolygon.v2 = tmp_vec4_1;
				l_pipelinePolygon->TransformedPolygon.v3 = tmp_vec4_2;
			}
		}

		// Backface culling
		{
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&p_memory->PolygonPipeline);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				PolygonPipeline* l_pipelinePolygon = l_polygonsIt.Current;

				Vector3<float> l_cameraToPolygon = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, p_input->CameraWorldPosition, &tmp_vec4_0));
				Vector3<float> l_u = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, &l_pipelinePolygon->TransformedPolygon.v2, &tmp_vec4_0));
				Vector3<float> l_v = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, &l_pipelinePolygon->TransformedPolygon.v3, &tmp_vec4_0));
				Vector3<float> l_n;
				VectorM::cross(&l_u, &l_v, &l_n);

				l_pipelinePolygon->IsCulled = VectorM::dot(&l_cameraToPolygon, &l_n) > 0.000f;
			}
		}

		{
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&p_memory->PolygonPipeline);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				Polygon<Vector4<float>> l_targetPolygon;
				PolygonPipeline* l_pipelinePolygon = l_polygonsIt.Current;

				if (!l_pipelinePolygon->IsCulled)
				{
					// World to camera
					MatrixM::mul(p_input->ViewMatrix, &l_pipelinePolygon->TransformedPolygon.v1, &tmp_vec4_0);
					MatrixM::mul(p_input->ViewMatrix, &l_pipelinePolygon->TransformedPolygon.v2, &tmp_vec4_1);
					MatrixM::mul(p_input->ViewMatrix, &l_pipelinePolygon->TransformedPolygon.v3, &tmp_vec4_2);

					// camera to clip
					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_0, &l_targetPolygon.v1);
					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_1, &l_targetPolygon.v2);
					MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_2, &l_targetPolygon.v3);

					VectorM::mul(&l_targetPolygon.v1, 1 / l_targetPolygon.v1.w, &l_targetPolygon.v1);
					VectorM::mul(&l_targetPolygon.v2, 1 / l_targetPolygon.v2.w, &l_targetPolygon.v2);
					VectorM::mul(&l_targetPolygon.v3, 1 / l_targetPolygon.v3.w, &l_targetPolygon.v3);

					//TODO -> Near clipping ? (https://gamedev.stackexchange.com/questions/22295/how-should-i-handling-clipping-vertices-that-are-closer-to-the-eye-than-the-near)

					// to screen pixel coord

					l_targetPolygon.v1.z = 1.0f; l_targetPolygon.v1.w = 1.0f;
					l_pipelinePolygon->TransformedPolygon.v1 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v1, &tmp_vec4_0));

					l_targetPolygon.v2.z = 1.0f; l_targetPolygon.v2.w = 1.0f;
					l_pipelinePolygon->TransformedPolygon.v2 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v2, &tmp_vec4_0));

					l_targetPolygon.v3.z = 1.0f; l_targetPolygon.v3.w = 1.0f;
					l_pipelinePolygon->TransformedPolygon.v3 = *(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v3, &tmp_vec4_0));
				}
			}
		}

		//Rasterize clip space polygons
		{
			{
				_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&p_memory->PolygonPipeline);
				while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
				{
					PolygonPipeline* l_pipelinePolygon = l_polygonsIt.Current;
					if (!l_pipelinePolygon->IsCulled)
					{
						Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v1, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v2, &p_memory->PixelDrawnCoordsBuffer);
						Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v2, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v3, &p_memory->PixelDrawnCoordsBuffer);
						Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v3, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v1, &p_memory->PixelDrawnCoordsBuffer);
					}

					_Core::VectorIteratorT<Vector2<int>> l_rasteriedPixel_it = _Core::VectorT_buildIterator(&p_memory->PixelDrawnCoordsBuffer);
					while (_Core::VectorIteratorT_moveNext(&l_rasteriedPixel_it))
					{
						if (l_rasteriedPixel_it.Current->x >= 0 && l_rasteriedPixel_it.Current->x < p_to->Width && l_rasteriedPixel_it.Current->y >= 0 && l_rasteriedPixel_it.Current->y < p_to->Height)
						{
							_Core::VectorT_pushBack(&p_memory->PixelsDrawn, l_rasteriedPixel_it.Current);
						}
					}
					_Core::VectorT_clear(&p_memory->PixelDrawnCoordsBuffer);
				}
			}
		}

		//Draw to texture
		{
			//TODO -> We can index data by pixel ?
			for (size_t i = 0; i < p_memory->PixelsDrawn.Size; i++)
			{
				_MathV2::Vector3<char> l_color = { 0, 0, 0 };
				_Core::VectorT_pushBack(&p_memory->PixelsDrawnColors, &l_color);
			}
			TextureM::writePixels(p_to, &p_memory->PixelsDrawn, &p_memory->PixelsDrawnColors);
		}

	};


	void WireframeRenderMemory_alloc(WireframeRenderMemory* p_memory)
	{
		_Core::VectorT_alloc(&p_memory->RenderableObjectsPipeline, 0);
		_Core::VectorT_alloc(&p_memory->PolygonPipeline, 0);
		_Core::VectorT_alloc(&p_memory->PixelDrawnCoordsBuffer, 0);
		_Core::VectorT_alloc(&p_memory->PixelsDrawn, 0);
		_Core::VectorT_alloc(&p_memory->PixelsDrawnColors, 0);
	};
	
	void WireframeRenderMemory_clear(WireframeRenderMemory* p_memory)
	{
		_Core::VectorT_clear(&p_memory->RenderableObjectsPipeline);
		_Core::VectorT_clear(&p_memory->PolygonPipeline);
		_Core::VectorT_clear(&p_memory->PixelDrawnCoordsBuffer);
		_Core::VectorT_clear(&p_memory->PixelsDrawn);
		_Core::VectorT_clear(&p_memory->PixelsDrawnColors);
	};
	
	void WireframeRenderMemory_free(WireframeRenderMemory* p_memory)
	{
		_Core::VectorT_free(&p_memory->RenderableObjectsPipeline);
		_Core::VectorT_free(&p_memory->PolygonPipeline);
		_Core::VectorT_free(&p_memory->PixelDrawnCoordsBuffer);
		_Core::VectorT_free(&p_memory->PixelsDrawn);
		_Core::VectorT_free(&p_memory->PixelsDrawnColors);
	};
}