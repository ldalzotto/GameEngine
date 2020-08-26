#include "WireframeRenderer.h"


#include "Cull/ObjectCulling.h"
#include "Cull/BackfaceCulling.h"
#include "Objects/Resource/Polygon.h"
#include "v2/_interface/MatrixC.h"
#include "Raster/Rasterizer.h"
#include "Renderer/Draw/DrawFunctions.h"
#include "DataStructures/ARRAY.h"
#include "Clock/Clock.h"


inline void Arr_PushBackRealloc_PolygonPipelineV2(ARRAY_PolygonPipelineV2_PTR p_arr, PolygonPipelineV2* p_polygon)
{
	Arr_PushBackRealloc(&p_arr->array, sizeof(PolygonPipelineV2), (char*)p_polygon);
};

void WireframeRenderer_renderV2(const WireframeRendererInput* p_input, Texture3c_PTR p_to, Recti_PTR p_to_clipRect, WireframeRenderer_Memory* p_memory)
{
	TimeClockPrecision l_before = Clock_currentTime_mics();

	WireframeRenderer_Memory_clear(p_memory, p_to->Width, p_to->Height);
	Vector3c l_wireframeColor = { 255,0,0 };
	Polygon4f tmp_poly_4f_0, tmp_poly_4f_1;

	{
		for (size_t i = 0; i < p_input->RenderableObjectsBuffer->RenderedObjects.Size; i++)
		{
			RenderedObject_PTR l_renderableObject = p_input->RenderableObjectsBuffer->RenderedObjects.Memory[i];

			{
				Matrix4f l_object_to_camera;
				Mat_Mul_M4F_M4F((Matrix4f_PTR)p_input->CameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

				if (!ObjectCulled_Boxf(l_renderableObject->MeshBoundingBox, (Matrix4f_PTR)&l_renderableObject->ModelMatrix, (Matrix4f_PTR)&l_object_to_camera, p_input->CameraBuffer->CameraFrustum))
				{
					// Push polygons
					for (size_t j = 0; j < l_renderableObject->Mesh->Polygons.Size; j++)
					{
						Polygon_VertexIndex_PTR l_polygon = &l_renderableObject->Mesh->Polygons.Memory[j];

						PolygonPipelineV2 l_polygonPipeline = {0};
						l_polygonPipeline.RenderedObject = l_renderableObject;
						l_polygonPipeline.PolygonIndex = j;

						l_polygonPipeline.TransformedPolygon.v1.Vec3 = l_renderableObject->Mesh->Vertices.Memory[l_polygon->v1].LocalPosition; l_polygonPipeline.TransformedPolygon.v1.Vec3_w = 1.0f;
						l_polygonPipeline.TransformedPolygon.v2.Vec3 = l_renderableObject->Mesh->Vertices.Memory[l_polygon->v2].LocalPosition; l_polygonPipeline.TransformedPolygon.v2.Vec3_w = 1.0f;
						l_polygonPipeline.TransformedPolygon.v3.Vec3 = l_renderableObject->Mesh->Vertices.Memory[l_polygon->v3].LocalPosition; l_polygonPipeline.TransformedPolygon.v3.Vec3_w = 1.0f;

						Arr_PushBackRealloc_PolygonPipelineV2(&p_memory->PolygonPipelines, &l_polygonPipeline);
					}

				}
			}
		}

		for (size_t i = 0; i < p_memory->PolygonPipelines.Size; i++)
		{
			PolygonPipelineV2* l_polygonPipeline = &p_memory->PolygonPipelines.Memory[i];

			// Local to world
			Polygon_Mul_V4F_M4F(&l_polygonPipeline->TransformedPolygon, &l_polygonPipeline->RenderedObject->ModelMatrix, &tmp_poly_4f_0);
			l_polygonPipeline->TransformedPolygon = tmp_poly_4f_0;

			// Backface culling
			if (BackFaceCulled_Poly4F(&l_polygonPipeline->TransformedPolygon, &p_input->CameraBuffer->WorldPosition))
			{
				continue;
			};

			//TODO -> We can combine matrices operations into one (for performance).

			// World to camera
			Polygon_Mul_V4F_M4F(&l_polygonPipeline->TransformedPolygon, p_input->CameraBuffer->ViewMatrix, &l_polygonPipeline->CameraSpacePolygon);

			// Camera to clip
			Polygon_MulHomogeneous_V4F_M4F(&l_polygonPipeline->CameraSpacePolygon, p_input->CameraBuffer->ProjectionMatrix, &l_polygonPipeline->TransformedPolygon);

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
				Polygon_Mul_V4F_M4F(&tmp_poly_4f_1, p_input->GraphicsAPIToScreeMatrix, &tmp_poly_4f_0);
				l_polygonPipeline->PixelPolygon = (Polygon2f) { tmp_poly_4f_0.v1.Vec3.Vec2, tmp_poly_4f_0.v2.Vec3.Vec2 , tmp_poly_4f_0.v3.Vec3.Vec2 };
			}

			// Rasterize
			{
				Draw_LineClipped(&l_polygonPipeline->PixelPolygon.v1, &l_polygonPipeline->PixelPolygon.v2, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
				Draw_LineClipped(&l_polygonPipeline->PixelPolygon.v2, &l_polygonPipeline->PixelPolygon.v3, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
				Draw_LineClipped(&l_polygonPipeline->PixelPolygon.v3, &l_polygonPipeline->PixelPolygon.v1, &p_memory->RasterizedPixelsBuffer, p_to, p_to_clipRect, &l_wireframeColor);
			}
		}


	}

	TimeClockPrecision l_after = Clock_currentTime_mics();
	printf("%d \n", l_after - l_before);
};


void WireframeRenderer_Memory_alloc(WireframeRenderer_Memory* p_memory)
{
	Arr_Alloc(&p_memory->PolygonPipelines.array, sizeof(PolygonPipelineV2), 0);
	Arr_Alloc_RasterisationStep(&p_memory->RasterizedPixelsBuffer, 0);
};
void WireframeRenderer_Memory_clear(WireframeRenderer_Memory* p_memory, size_t p_width, size_t height)
{
	Arr_Clear(&p_memory->PolygonPipelines.array);
	Arr_Clear_RasterisationStep(&p_memory->RasterizedPixelsBuffer);
	Arr_Resize_RasterisationStep(&p_memory->RasterizedPixelsBuffer, p_width > height ? p_width * 2 : height * 2);

};
void WireframeRenderer_Memory_free(WireframeRenderer_Memory* p_memory)
{
	Arr_Free(&p_memory->PolygonPipelines.array);
	Arr_Free_RasterisationStep(&p_memory->RasterizedPixelsBuffer);
};