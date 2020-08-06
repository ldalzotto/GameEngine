#include "WireframeRenderer.hpp"

#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

#include "v2/Matrix/MatrixMath.hpp"
#include "v2/Vector/VectorMath.hpp"

#include "Objects/Polygon.hpp"
#include "Objects/Vertex.hpp"

using namespace _MathV2;

namespace _RenderV2
{
	void WirerameRenderer_render(WireframeRendererInput* p_input)
	{
		_Core::ArrayT<Polygon<Vector4<float>>> l_clipSpacePolygons;
		_Core::ArrayT_alloc(&l_clipSpacePolygons, p_input->Polygons->Size);
		l_clipSpacePolygons.Size = l_clipSpacePolygons.Capacity;

		{
			_Core::VectorIteratorT<PolygonInput> l_polygonsIt = _Core::VectorT_buildIterator(p_input->Polygons);
			while (_Core::VectorIteratorT_moveNext(&l_polygonsIt))
			{
				Polygon<Vector4<float>>* l_targetPolygon = _Core::ArrayT_at(&l_clipSpacePolygons, l_polygonsIt.CurrentIndex);

				_MathV2::Vector4<float> tmp_vec4_0, tmp_vec4_1, tmp_vec4_2, tmp_vec4_3, tmp_vec4_4, tmp_vec4_5;

				// Local to world
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v1->LocalPosition, 1.0f), &tmp_vec4_0);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v2->LocalPosition, 1.0f), &tmp_vec4_1);
				MatrixM::mul(l_polygonsIt.Current->ModelMatrix, &VectorM::cast(&l_polygonsIt.Current->Polygon->v3->LocalPosition, 1.0f), &tmp_vec4_2);

				// World to camera
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_0, &tmp_vec4_3);
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_1, &tmp_vec4_4);
				MatrixM::mul(p_input->ViewMatrix, &tmp_vec4_2, &tmp_vec4_5);

				// camera to clip
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_3, &l_targetPolygon->v1);
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_4, &l_targetPolygon->v2);
				MatrixM::mul(p_input->ProjectionMatrix, &tmp_vec4_5, &l_targetPolygon->v3);

				_MathV2::VectorM::mul(&l_targetPolygon->v1, 1 / l_targetPolygon->v1.w, &l_targetPolygon->v1);
				_MathV2::VectorM::mul(&l_targetPolygon->v2, 1 / l_targetPolygon->v2.w, &l_targetPolygon->v2);
				_MathV2::VectorM::mul(&l_targetPolygon->v3, 1 / l_targetPolygon->v3.w, &l_targetPolygon->v3);
			}
		}




		_Core::ArrayT_free(&l_clipSpacePolygons);
	};
}