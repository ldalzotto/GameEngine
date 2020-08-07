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
	void WirerameRenderer_render(const WireframeRendererInput* p_input, Texture<3, char>* p_to)
	{
		_Core::ArrayT<Polygon<Vector2<float>>> l_clipSpacePolygons;
		_Core::ArrayT_alloc(&l_clipSpacePolygons, p_input->Polygons->Size);
		l_clipSpacePolygons.Size = l_clipSpacePolygons.Capacity;

		Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2, tmp_vec4_3, tmp_vec4_4, tmp_vec4_5;

		{
			_Core::VectorIteratorT<PolygonInput> l_polygonsIt = _Core::VectorT_buildIterator(p_input->Polygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				Polygon<Vector4<float>> l_targetPolygon;
				Polygon<Vector2<float>>* l_screenSpacePolygon = _Core::ArrayT_at(&l_clipSpacePolygons, l_polygonsIt.CurrentIndex);

				// Local to world
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v1->LocalPosition, 1.0f), &tmp_vec4_0);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v2->LocalPosition, 1.0f), &tmp_vec4_1);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v3->LocalPosition, 1.0f), &tmp_vec4_2);

				// World to camera
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_0, &tmp_vec4_3);
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_1, &tmp_vec4_4);
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_2, &tmp_vec4_5);

				// camera to clip
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_3, &l_targetPolygon.v1);
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_4, &l_targetPolygon.v2);
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_5, &l_targetPolygon.v3);

				VectorM::mul(&l_targetPolygon.v1, 1 / l_targetPolygon.v1.w, &l_targetPolygon.v1);
				VectorM::mul(&l_targetPolygon.v2, 1 / l_targetPolygon.v2.w, &l_targetPolygon.v2);
				VectorM::mul(&l_targetPolygon.v3, 1 / l_targetPolygon.v3.w, &l_targetPolygon.v3);

				// to screen pixel coord

				l_targetPolygon.v1.z = 1.0f; l_targetPolygon.v1.w = 1.0f;
				l_screenSpacePolygon->v1 = *VectorM::cast2(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v1, &tmp_vec4_0));

				l_targetPolygon.v2.z = 1.0f; l_targetPolygon.v2.w = 1.0f;
				l_screenSpacePolygon->v2 = *VectorM::cast2(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v2, &tmp_vec4_0));

				l_targetPolygon.v3.z = 1.0f; l_targetPolygon.v3.w = 1.0f;
				l_screenSpacePolygon->v3 = *VectorM::cast2(MatrixM::mul(p_input->GraphicsAPIToScreeMatrix, &l_targetPolygon.v3, &tmp_vec4_0));
			}
		}

		_Core::VectorT<_MathV2::Vector2<int>> l_pixelsDrawn;
		_Core::VectorT_alloc(&l_pixelsDrawn, 0);
		//Rasterize clip space polygons
		{
			_Core::VectorIteratorT<Polygon<Vector2<float>>> l_clipSpacePolygon_it = _Core::ArrayT_buildIterator(&l_clipSpacePolygons);
			while (_Core::VectorIteratorT_moveNext(&l_clipSpacePolygon_it))
			{
				Polygon<Vector2<float>>* l_clipSpacePolygon = l_clipSpacePolygon_it.Current;
				Rasterizer::line(&l_clipSpacePolygon->v1, &l_clipSpacePolygon->v2, &l_pixelsDrawn);
				Rasterizer::line(&l_clipSpacePolygon->v2, &l_clipSpacePolygon->v3, &l_pixelsDrawn);
				Rasterizer::line(&l_clipSpacePolygon->v3, &l_clipSpacePolygon->v1, &l_pixelsDrawn);
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
		_Core::ArrayT_free(&l_clipSpacePolygons);
	};
}