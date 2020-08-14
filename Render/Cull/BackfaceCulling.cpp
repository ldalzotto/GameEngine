#include "BackfaceCulling.hpp"

#include "Constants.hpp"
#include "Objects/Resource/Polygon.hpp"
#include "v2/Vector/VectorMath.hpp"

using namespace _MathV2;

namespace _RenderV2
{
	bool BackfaceCullingM::isCulled(const Polygon<_MathV2::Vector<4, float>>* p_polygon, const _MathV2::Vector<4, float>* p_cameraWorldPosition)
	{
		Vector<4, float> tmp_vec4_0;

		Vector3<float> l_cameraToPolygon = *VectorM::cast(VectorM::min(&p_polygon->v1, p_cameraWorldPosition, &tmp_vec4_0));
		Vector3<float> l_u = *VectorM::cast(VectorM::min(&p_polygon->v1, &p_polygon->v2, &tmp_vec4_0));
		Vector3<float> l_v = *VectorM::cast(VectorM::min(&p_polygon->v1, &p_polygon->v3, &tmp_vec4_0));
		Vector3<float> l_n;
		VectorM::cross(&l_u, &l_v, &l_n);

		return VectorM::dot(&l_cameraToPolygon, &l_n) > FLOAT_TOLERANCE;
	};
}