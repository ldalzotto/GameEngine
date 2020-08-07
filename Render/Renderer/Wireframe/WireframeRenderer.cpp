#include "WireframeRenderer.hpp"

#include <cmath>

#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"

#include "Objects/Polygon.hpp"
#include "Objects/Vertex.hpp"
#include "Objects/Mesh.hpp"
#include "Objects/Texture/TextureM.hpp"

#include "Raster/Rasterizer.hpp"


using namespace _MathV2;

namespace _RenderV2
{
	struct PolygonPipeline
	{
		bool IsCulled;
		Polygon<Vector4<float>> TransformedPolygon;
		_MathV2::Matrix<4, 4, float>* ModelMatrix;
	};

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to)
	{
		//TODO -> Object culling
		_Core::VectorT<RenderableObject>* l_renderedObjects = p_input->RenderableObjects;

		_Core::VectorT<PolygonPipeline> l_transformedPolygons;
		_Core::VectorT_alloc(&l_transformedPolygons, 0);

		// Creating polygon list
		{
			_Core::VectorIteratorT<RenderableObject> l_renderedObjects_it = _Core::VectorT_buildIterator(l_renderedObjects);
			while (_Core::VectorIteratorT_moveNext(&l_renderedObjects_it))
			{
				_Core::VectorIteratorT<Polygon<Vertex*>> l_meshPolygons_it = _Core::ArrayT_buildIterator(&l_renderedObjects_it.Current->Mesh->Polygons);
				while (_Core::VectorIteratorT_moveNext(&l_meshPolygons_it))
				{
					PolygonPipeline l_polygon{};
					l_polygon.ModelMatrix = l_renderedObjects_it.Current->ModelMatrix;
					l_polygon.TransformedPolygon = {
						VectorM::cast(&l_meshPolygons_it.Current->v1->LocalPosition, 1.0f),
						VectorM::cast(&l_meshPolygons_it.Current->v2->LocalPosition, 1.0f),
						VectorM::cast(&l_meshPolygons_it.Current->v3->LocalPosition, 1.0f)
					};
					_Core::VectorT_pushBack(&l_transformedPolygons, &l_polygon);
				}
			}
		}
		
		Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2;

		{
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&l_transformedPolygons);
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
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&l_transformedPolygons);
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
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&l_transformedPolygons);
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

		_Core::VectorT<_MathV2::Vector2<int>> l_pixelsDrawn;
		_Core::VectorT_alloc(&l_pixelsDrawn, 0);
		//Rasterize clip space polygons
		{
			_Core::VectorIteratorT<PolygonPipeline> l_polygonsIt = _Core::VectorT_buildIterator(&l_transformedPolygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				PolygonPipeline* l_pipelinePolygon = l_polygonsIt.Current;
				if (!l_pipelinePolygon->IsCulled)
				{
					Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v1, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v2, &l_pixelsDrawn);
					Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v2, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v3, &l_pixelsDrawn);
					Rasterizer::line((Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v3, (Vector2<float>*) & l_pipelinePolygon->TransformedPolygon.v1, &l_pixelsDrawn);
				}
			}
		}

		_Core::VectorT_free(&l_transformedPolygons);

		//Draw to texture
		{
			_Core::VectorT<_MathV2::Vector3<char>> p_drawnColors{};
			_Core::VectorT_alloc(&p_drawnColors, l_pixelsDrawn.Size);
			for (size_t i = 0; i < l_pixelsDrawn.Size; i++)
			{
				_Core::VectorT_pushBack(&p_drawnColors, { (char)255,(char)255, (char)255 });
			}
			TextureM::writePixels(p_to, &l_pixelsDrawn, &p_drawnColors);
			_Core::VectorT_free(&p_drawnColors);
		}

		_Core::VectorT_free(&l_pixelsDrawn);
	};
}