#include "WireframeRenderer.hpp"

#include "Cull/ObjectCulling.hpp"
#include "Cull/BackfaceCulling.hpp"

#include "Objects/Resource/Mesh.hpp"
#include "Objects/RenderedObject.hpp"
#include "Objects/Resource/PolygonMethods.hpp"
#include "Objects/Texture/TextureM.hpp"

#include "Renderer/Draw/DrawFunctions.hpp"

#include "Renderer/GlobalBuffers/CameraBuffer.hpp"
#include "Renderer/GlobalBuffers/RenderedObjectsBuffer.hpp"

#include "Clock/Clock.hpp"
#include <iostream>

using namespace _MathV2;

namespace _RenderV2
{
	void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture<3, char>* p_to, _MathV2::Rect<int>* p_to_clipRect, WireframeRenderer_Memory* p_memory)
	{
		_Core::TimeClockPrecision l_before = _Core::Clock_currentTime_mics();

		WireframeRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);
		_MathV2::Vector<3, char> l_wireframeColor = { 255,0,0 };
		Polygon<Vector<4, float>> tmp_poly_4f_0, tmp_poly_4f_1;

		{
			for (size_t i = 0; i < p_input->RenderableObjectsBuffer->RenderedObjects.Size; i++)
			{
				RenderedObject* l_renderableObject = p_input->RenderableObjectsBuffer->RenderedObjects.Memory[i];

				{
					MATRIX4F l_object_to_camera;
					Mat_Mul_M4F_M4F((MATRIX4F_PTR)p_input->CameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

					if (!ObjectCullingM::isObjectCulled(l_renderableObject->MeshBoundingBox, (MATRIX4F_PTR)&l_renderableObject->ModelMatrix, (MATRIX4F_PTR)&l_object_to_camera, p_input->CameraBuffer->CameraFrustum))
					{
						// Push polygons
						for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
						{
							Polygon<VertexIndex>* l_polygon = &l_renderableObject->Mesh->Polygons.Memory[j];

							PolygonPipelineV2 l_polygonPipeline{};
							l_polygonPipeline.RenderedObject = l_renderableObject;
							l_polygonPipeline.PolygonIndex = j;

							l_polygonPipeline.TransformedPolygon = {
								VectorM::cast(&l_renderableObject->Mesh->Vertices.Memory[l_polygon->v1].LocalPosition, 1.0f),
								VectorM::cast(&l_renderableObject->Mesh->Vertices.Memory[l_polygon->v2].LocalPosition, 1.0f),
								VectorM::cast(&l_renderableObject->Mesh->Vertices.Memory[l_polygon->v3].LocalPosition, 1.0f)
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
				l_polygonPipeline->TransformedPolygon = *PolygonM::mul(&l_polygonPipeline->TransformedPolygon, &l_polygonPipeline->RenderedObject->ModelMatrix, &tmp_poly_4f_0);

				// Backface culling
				if (BackfaceCullingM::isCulled(&l_polygonPipeline->TransformedPolygon, &p_input->CameraBuffer->WorldPosition))
				{
					continue;
				};

				//TODO -> We can combine matrices operations into one (for performance).

				// World to camera
				PolygonM::mul(&l_polygonPipeline->TransformedPolygon, p_input->CameraBuffer->ViewMatrix, &l_polygonPipeline->CameraSpacePolygon);

				// Camera to clip
				PolygonM::mul_homogeneous(&l_polygonPipeline->CameraSpacePolygon, p_input->CameraBuffer->ProjectionMatrix, &l_polygonPipeline->TransformedPolygon);

#if 0
				// This check prevents prolygon that have vertex behind the camera to be rendered
				if (l_polygonPipeline->TransformedPolygon.v1.z > 1.0f ||
					l_polygonPipeline->TransformedPolygon.v2.z > 1.0f ||
					l_polygonPipeline->TransformedPolygon.v3.z > 1.0f)
				{
					continue;
				}
#endif
				// To pixel
				{
					tmp_poly_4f_1 = l_polygonPipeline->TransformedPolygon;
					tmp_poly_4f_1.v1.z = 1.0f; tmp_poly_4f_1.v2.z = 1.0f; tmp_poly_4f_1.v3.z = 1.0f;
					l_polygonPipeline->PixelPolygon = PolygonM::cast_value_2(PolygonM::mul(&tmp_poly_4f_1, (MATRIX4F_PTR)p_input->GraphicsAPIToScreeMatrix, &tmp_poly_4f_0));
				}

				// Rasterize
				{
					DrawM::DrawLineClipped(&l_polygonPipeline->PixelPolygon.v1, &l_polygonPipeline->PixelPolygon.v2, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
					DrawM::DrawLineClipped(&l_polygonPipeline->PixelPolygon.v2, &l_polygonPipeline->PixelPolygon.v3, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
					DrawM::DrawLineClipped(&l_polygonPipeline->PixelPolygon.v3, &l_polygonPipeline->PixelPolygon.v1, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
				}
			}


		}

		_Core::TimeClockPrecision l_after = _Core::Clock_currentTime_mics();
		std::cout << l_after - l_before << std::endl;
	};


	void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory)
	{
		_Core::VectorT_alloc(&p_memory->PolygonPipelines, 0);
		_Core::VectorT_alloc(&p_memory->RasterizedPixelsBuffer, 0);
	};
	void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height)
	{
		_Core::VectorT_clear(&p_memory->PolygonPipelines);
		_Core::VectorT_clear(&p_memory->RasterizedPixelsBuffer);
		_Core::VectorT_resize(&p_memory->RasterizedPixelsBuffer, p_width > height ? p_width * 2 : height * 2);

	};
	void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory)
	{
		_Core::VectorT_free(&p_memory->PolygonPipelines);
		_Core::VectorT_free(&p_memory->RasterizedPixelsBuffer);
	};
}