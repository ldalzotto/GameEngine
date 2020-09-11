#pragma once


#include "BoxC_alg_impl.h"
#include "v2/_interface/VectorC.h"

#define BOX_BUILD_ALROGITHM(GET_POINT_LOOP_BEGIN, GET_POINT_LOOP_END, GET_POINT_COUNT) \
Vector3f l_approximateCenter = Vector3f_ZERO; \
{ \
\
	GET_POINT_LOOP_BEGIN \
		Vec_Add_3f_3f(&l_approximateCenter, l_point, &l_approximateCenter); \
	GET_POINT_LOOP_END \
	Vec_Mul_3f_1f(&l_approximateCenter, 1.0f / GET_POINT_COUNT, &l_approximateCenter); \
} \
\
Vector3f l_min = l_approximateCenter, l_max = l_approximateCenter; \
{ \
	GET_POINT_LOOP_BEGIN \
\
		if (l_point->x <= l_min.x) { l_min.x = l_point->x; } \
		if (l_point->y <= l_min.y) { l_min.y = l_point->y; } \
		if (l_point->z <= l_min.z) { l_min.z = l_point->z; } \
\
		if (l_point->x >= l_max.x) { l_max.x = l_point->x; } \
		if (l_point->y >= l_max.y) { l_max.y = l_point->y; } \
		if (l_point->z >= l_max.z) { l_max.z = l_point->z; } \
	GET_POINT_LOOP_END \
\
} \
\
{ \
	p_box->Center = Vector3f_ZERO; \
\
	Vec_Add_3f_3f(&p_box->Center, &l_min, &p_box->Center);\
	Vec_Add_3f_3f(&p_box->Center, &l_max, &p_box->Center);\
	Vec_Mul_3f_1f(&p_box->Center, 0.5f, &p_box->Center); \
} \
\
Box_Grow_F(p_box, &l_min); \
Box_Grow_F(p_box, &l_max);