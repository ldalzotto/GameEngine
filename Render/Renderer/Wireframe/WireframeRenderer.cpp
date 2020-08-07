#include "WireframeRenderer.hpp"

#include <cmath>

#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"

#include "Objects/Polygon.hpp"
#include "Objects/Vertex.hpp"
#include "Objects/Texture/TextureM.hpp"

#include "Raster/Rasterizer.hpp"


using namespace _MathV2;

namespace _RenderV2
{
	struct PolygonPipeline
	{
		bool IsCulled;
		Polygon<Vector4<float>> TransformedPolygon;
	};

	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to)
	{
		_Core::ArrayT<PolygonPipeline> l_transformedPolygons;
		_Core::ArrayT_alloc(&l_transformedPolygons, p_input->Polygons->Size);
		_Core::ArrayT_zeroing(&l_transformedPolygons);
		l_transformedPolygons.Size = l_transformedPolygons.Capacity;

		Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2;

		{
			_Core::VectorIteratorT<PolygonInput> l_polygonsIt = _Core::VectorT_buildIterator(p_input->Polygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				PolygonPipeline* l_pipelinePolygon = _Core::ArrayT_at(&l_transformedPolygons, l_polygonsIt.CurrentIndex);

				// Local to world
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v1->LocalPosition, 1.0f), &tmp_vec4_0);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v2->LocalPosition, 1.0f), &tmp_vec4_1);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v3->LocalPosition, 1.0f), &tmp_vec4_2);

				l_pipelinePolygon->TransformedPolygon.v1 = tmp_vec4_0;
				l_pipelinePolygon->TransformedPolygon.v2 = tmp_vec4_1;
				l_pipelinePolygon->TransformedPolygon.v3 = tmp_vec4_2;
			}
		}

		// Backface culling
		{
			_Core::VectorIteratorT<PolygonInput> l_polygonsIt = _Core::VectorT_buildIterator(p_input->Polygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				PolygonPipeline* l_pipelinePolygon = _Core::ArrayT_at(&l_transformedPolygons, l_polygonsIt.CurrentIndex);

				Vector3<float> l_cameraToPolygon = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, p_input->CameraWorldPosition, &tmp_vec4_0));
				Vector3<float> l_u = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, &l_pipelinePolygon->TransformedPolygon.v2, &tmp_vec4_0));
				Vector3<float> l_v = *VectorM::cast(VectorM::min(&l_pipelinePolygon->TransformedPolygon.v1, &l_pipelinePolygon->TransformedPolygon.v3, &tmp_vec4_0));
				Vector3<float> l_n;
				VectorM::cross(&l_u, &l_v, &l_n);

				l_pipelinePolygon->IsCulled = VectorM::dot(&l_cameraToPolygon, &l_n) > 0.000f;
			}
		}

		{
			_Core::VectorIteratorT<PolygonInput> l_polygonsIt = _Core::VectorT_buildIterator(p_input->Polygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				Polygon<Vector4<float>> l_targetPolygon;
				PolygonPipeline* l_pipelinePolygon = _Core::ArrayT_at(&l_transformedPolygons, l_polygonsIt.CurrentIndex);

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
			_Core::VectorIteratorT<PolygonPipeline> l_clipSpacePolygon_it = _Core::ArrayT_buildIterator(&l_transformedPolygons);
			while (_Core::VectorIteratorT_moveNext(&l_clipSpacePolygon_it))
			{
				PolygonPipeline* l_clipSpacePolygon = l_clipSpacePolygon_it.Current;
				if (!l_clipSpacePolygon->IsCulled)
				{
					Rasterizer::line((Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v1, (Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v2, &l_pixelsDrawn);
					Rasterizer::line((Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v2, (Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v3, &l_pixelsDrawn);
					Rasterizer::line((Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v3, (Vector2<float>*) & l_clipSpacePolygon->TransformedPolygon.v1, &l_pixelsDrawn);
				}
			}
		}

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
		_Core::ArrayT_free(&l_transformedPolygons);
	};
}