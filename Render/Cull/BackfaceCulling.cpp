#include "BackfaceCulling.hpp"

#include "Constants.h"
#include "Objects/Resource/Polygon.hpp"

extern "C"
{
#include "v2/_interface/VectorC.h"
}

namespace _RenderV2
{
	bool BackfaceCullingM::isCulled(const Polygon<VECTOR4F>* p_polygon, const VECTOR4F_PTR p_cameraWorldPosition)
	{
		VECTOR4F tmp_vec4_0;

		Vec_Min_4f_4f((VECTOR4F_PTR)&p_polygon->v1, p_cameraWorldPosition, &tmp_vec4_0);
		VECTOR3F l_cameraToPolygon = tmp_vec4_0.Vec3;
		Vec_Min_4f_4f((VECTOR4F_PTR)&p_polygon->v1, (VECTOR4F_PTR)&p_polygon->v2, &tmp_vec4_0);
		VECTOR3F l_u = tmp_vec4_0.Vec3;
		Vec_Min_4f_4f((VECTOR4F_PTR)&p_polygon->v1, (VECTOR4F_PTR)&p_polygon->v3, &tmp_vec4_0);
		VECTOR3F l_v = tmp_vec4_0.Vec3;
		VECTOR3F l_n;
		Vec_Cross_3f(&l_u, &l_v, &l_n);

		return Vec_Dot_3f(&l_cameraToPolygon, &l_n) > FLOAT_TOLERANCE;
	};
}