#include "SolidRendererV2.h"
#include "v2/_interface/MatrixC.h"
#include "Cull/ObjectCulling.h"
#include "Heap/RenderHeap_def.h"
#include "Objects/Resource/Polygon.h"
#include "Cull/BackfaceCulling.h"
#include "v2/_interface/WindowSize.h"
#include "Raster/Rasterizer.h"
#include "Depth/DepthBuffer.h"
#include "Light/Light_def.h"
#include "Objects/Resource/Polygon_inline.c"
#include "Objects/Texture/TextureSampling_inline.c"

// There is a unique directional light


inline void _i_NoShade_Textured_Perspective(const Polygon2i_PTR p_polygonTexCoords,
	RenderLights_PTR p_renderLights, Material_PTR p_material, Texture3c_PTR p_materialDiffuseTexture,
	const PolygonRasterizer_InterpolationFactor_PTR p_interpolationFactors, PolygonPerspectiveInterpolation_PTR p_perspectiveInterpolation, Color3f_PTR out_pixelColor)
{

	Vector2i l_interpolatedCoordinates;
	Color3f l_sampledPoint;

	_i_Polygon_Interpolate_Perspective_V2i(p_polygonTexCoords, p_interpolationFactors->I0, p_interpolationFactors->I1, p_interpolationFactors->I2, p_perspectiveInterpolation, &l_interpolatedCoordinates);
	_i_TextureSample_Point_Coords_3f(p_materialDiffuseTexture, &l_interpolatedCoordinates, &l_sampledPoint);

	out_pixelColor->Vec.x = (p_material->BaseColor.r * l_sampledPoint.r) /*+ (p_renderLights->AmbientLight.Color.r)*/;
	out_pixelColor->Vec.y = (p_material->BaseColor.g * l_sampledPoint.g) /*+ (p_renderLights->AmbientLight.Color.g)*/;
	out_pixelColor->Vec.z = (p_material->BaseColor.b * l_sampledPoint.b) /*+ (p_renderLights->AmbientLight.Color.b)*/;

};

inline char _i_DepthTest_Perspective(PolygonRasterizerIterator_CommonStructure_PTR p_rasterizer, DepthBuffer_PTR p_depthBuffer,
	PolygonPerspectiveInterpolation_PTR p_polygonPerspectiveInterpolation)
{
	p_polygonPerspectiveInterpolation->ZValueInv = ((p_polygonPerspectiveInterpolation->InvertedZValueOnPolygon.v1 * p_rasterizer->InterpolationFactors.I0)
		+ (p_polygonPerspectiveInterpolation->InvertedZValueOnPolygon.v2 * p_rasterizer->InterpolationFactors.I1) + (p_polygonPerspectiveInterpolation->InvertedZValueOnPolygon.v3 * p_rasterizer->InterpolationFactors.I2));
	p_polygonPerspectiveInterpolation->ZValue = 1.0f / p_polygonPerspectiveInterpolation->ZValueInv;

	// Pixel depth culling
	return DepthBuffer_PushDepthValue(p_depthBuffer, &p_rasterizer->RasterizedPixel, p_polygonPerspectiveInterpolation->ZValue) /* && p_polygonPerspectiveInterpolation->ZValue > 0.0f */;
}

void DrawObjects_NoShade_Textured_V2(const SolidRendererInput* p_input, RenderTexture3f_PTR p_to, DepthBuffer_PTR p_depthBuffer, RendererPipeline_Memory_PTR p_memory)
{
	for (size_t i = 0; i < p_input->RenderableObjectsBuffer->RenderedObjects.Size; i++)
	{
		RenderedObject_PTR l_renderableObject = p_input->RenderableObjectsBuffer->RenderedObjects.Memory[i];
		Matrix4f l_object_to_camera;
		Mat_Mul_M4F_M4F((Matrix4f_PTR)p_input->CameraBuffer->ViewMatrix, &l_renderableObject->ModelMatrix, &l_object_to_camera);

		if (!ObjectCulled_Boxf(l_renderableObject->MeshBoundingBox, (Matrix4f_PTR)&l_renderableObject->ModelMatrix, (Matrix4f_PTR)&l_object_to_camera, p_input->CameraBuffer->CameraFrustum))
		{
			Mesh_PTR l_mesh = l_renderableObject->Mesh;
			Material_PTR l_material = &RRenderHeap.MaterialAllocator.array.Memory[l_renderableObject->Material.Handle];
			Texture3c_PTR l_materialDiffuseTexture = &RRenderHeap.Texture3cAllocator.array.Memory[l_material->DiffuseTexture.Handle];

			for (size_t j = 0; j < l_mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_vertexIndex = &RRenderHeap.PolygonAllocator.array.Memory[j];
				Vertex_PTR l_v1 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v1];
				Vertex_PTR l_v2 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v2];
				Vertex_PTR l_v3 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v3];

				l_v1->WorldPositionCalcualted = 0; l_v1->PixelPositionCalcualted = 0;
				l_v2->WorldPositionCalcualted = 0; l_v2->PixelPositionCalcualted = 0;
				l_v3->WorldPositionCalcualted = 0; l_v3->PixelPositionCalcualted = 0;
			}


			for (size_t j = 0; j < l_mesh->Polygons.Size; j++)
			{
				Polygon_VertexIndex_PTR l_vertexIndex = &RRenderHeap.PolygonAllocator.array.Memory[j];
				Vertex_PTR l_v1 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v1];
				Vertex_PTR l_v2 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v2];
				Vertex_PTR l_v3 = &RRenderHeap.VertexAllocator.array.Memory[l_vertexIndex->v3];


				// Local to world
				if (!l_v1->WorldPositionCalcualted) { Mat_Mul_M4F_V4F(&l_renderableObject->ModelMatrix, &l_v1->LocalPosition, &l_v1->WorldPosition); l_v1->WorldPositionCalcualted = 1; }
				if (!l_v2->WorldPositionCalcualted) { Mat_Mul_M4F_V4F(&l_renderableObject->ModelMatrix, &l_v2->LocalPosition, &l_v2->WorldPosition); l_v2->WorldPositionCalcualted = 1; }
				if (!l_v3->WorldPositionCalcualted) { Mat_Mul_M4F_V4F(&l_renderableObject->ModelMatrix, &l_v3->LocalPosition, &l_v3->WorldPosition); l_v3->WorldPositionCalcualted = 1; }

				// Backface culling
				Polygon4fPTR l_worldPositionPoly =
				{
					.v1 = &l_v1->WorldPosition,
					.v2 = &l_v2->WorldPosition,
					.v3 = &l_v3->WorldPosition
				};

				Vector3f l_polygon_worldNormal;

				Polygon_CalculateNormal_V3FPTR(&l_worldPositionPoly, &l_polygon_worldNormal);

				if (!BackFaceCulled_Normal3fPTR(&l_polygon_worldNormal, l_worldPositionPoly.v1, &p_input->CameraBuffer->WorldPosition))
				{
					// World to Camera
					Vector4f l_clipPosition;
					if (!l_v1->PixelPositionCalcualted)
					{
						Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v1->WorldPosition, &l_v1->CameraSpacePosition);
						Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_v1->CameraSpacePosition, &l_clipPosition);
						WindowSize_GraphicsAPIToPixel(&p_to->PrecalculatedDimensions, l_clipPosition.x, l_clipPosition.y, &l_v1->PixelPosition.x, &l_v1->PixelPosition.y);
						l_v1->PixelPositionCalcualted = 1;
					}
					if (!l_v2->PixelPositionCalcualted)
					{
						Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v2->WorldPosition, &l_v2->CameraSpacePosition);
						Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_v2->CameraSpacePosition, &l_clipPosition);
						WindowSize_GraphicsAPIToPixel(&p_to->PrecalculatedDimensions, l_clipPosition.x, l_clipPosition.y, &l_v2->PixelPosition.x, &l_v2->PixelPosition.y);
						l_v2->PixelPositionCalcualted = 1;
					}
					if (!l_v3->PixelPositionCalcualted)
					{
						Mat_Mul_M4F_V4F(p_input->CameraBuffer->ViewMatrix, &l_v3->WorldPosition, &l_v3->CameraSpacePosition);
						Mat_Mul_M4F_V4F_Homogeneous(p_input->CameraBuffer->ProjectionMatrix, &l_v3->CameraSpacePosition, &l_clipPosition);
						WindowSize_GraphicsAPIToPixel(&p_to->PrecalculatedDimensions, l_clipPosition.x, l_clipPosition.y, &l_v3->PixelPosition.x, &l_v3->PixelPosition.y);
						l_v3->PixelPositionCalcualted = 1;
					}


					// We skip polygon that have at least one vertex behind the camera
					// TODO -> This check must be removed and instead, we manually create polygon that cutted by the camera near plane.
					if (l_v1->CameraSpacePosition.z > 0.0f && l_v2->CameraSpacePosition.z > 0.0f && l_v3->CameraSpacePosition.z > 0.0f)
					{





						Color3f l_pixelColor;
						Polygon2i l_difuseTextureCoord_polygon;
				
						Polygon_UV_PTR l_polygonUV = &RRenderHeap.PolygonUVAllocator.array.Memory[j];

						// Instead of calculating texture coordinate for every pixels, we calculate it once for the polygon, then interpolate it.
						float l_diffuseWidth_min1 = l_materialDiffuseTexture->Width - 1.0f;
						float l_diffuseHeight_min1 = l_materialDiffuseTexture->Width - 1.0f;

						l_difuseTextureCoord_polygon = (Polygon2i){
							(Vector2i) {(l_polygonUV->v1.x * l_diffuseWidth_min1), ((1.0f - l_polygonUV->v1.y) * l_diffuseHeight_min1)},
							(Vector2i) {(l_polygonUV->v2.x * l_diffuseWidth_min1), ((1.0f - l_polygonUV->v2.y) * l_diffuseHeight_min1)},
							(Vector2i) {(l_polygonUV->v3.x * l_diffuseWidth_min1), ((1.0f - l_polygonUV->v3.y) * l_diffuseHeight_min1)},
						};

						// Rasterize
						Polygon2i l_pixelPositionPolygon = (Polygon2i)
						{
								.v1 = l_v1->PixelPosition,
								.v2 = l_v2->PixelPosition,
								.v3 = l_v3->PixelPosition
						};

						PolygonPerspectiveInterpolation l_perspectiveInterpolation;
						l_perspectiveInterpolation.InvertedZValueOnPolygon = (Polygonf)
						{
								.v1 = 1.0f / l_v1->CameraSpacePosition.z,
								.v2 = 1.0f / l_v2->CameraSpacePosition.z,
								.v3 = 1.0f / l_v3->CameraSpacePosition.z
						};

						PolygonRasterizeSmartIterator l_rasterizerIterator;
						PolygonRasterizeSmart_Initialize(&l_pixelPositionPolygon, &p_to->BoundingRectangle, &l_rasterizerIterator);

						POLYGONRASTERIZER_ITERATOR_RETURN_CODE l_returnCode = POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_NOT_RASTERIZED;
						while (l_returnCode != POLYGONRASTERIZER_ITERATOR_RETURN_CODE_END)
						{
							l_returnCode = PolygonRasterizeSmart_MoveNext_Interpolated(&l_rasterizerIterator);

							if (l_returnCode == POLYGONRASTERIZER_ITERATOR_RETURN_CODE_PIXEL_RASTERIZED)
							{
								if (_i_DepthTest_Perspective(&l_rasterizerIterator.CommonStructure, p_depthBuffer, &l_perspectiveInterpolation))
								{
									_i_NoShade_Textured_Perspective(&l_difuseTextureCoord_polygon, NULL, l_material, l_materialDiffuseTexture, &l_rasterizerIterator.CommonStructure.InterpolationFactors, &l_perspectiveInterpolation,
										&l_pixelColor);
									p_to->Texture.Pixels.Memory[l_rasterizerIterator.CommonStructure.RasterizedPixel.x + (l_rasterizerIterator.CommonStructure.RasterizedPixel.y * p_to->PrecalculatedDimensions.Width)] = l_pixelColor;
								}
							}
						}



					}
				}
			}
		}
	}
};