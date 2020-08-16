
extern "C"
{
	#include "v2/_source/VectorC.h"
}

int main()
{
	VECTOR3F l_vec = {0.0f, 1.0f, 2.0f};
	VECTOR3F l_vec2 = { 0.0f, 1.0f, 2.0f };
	VECTOR3F l_out;

	// Vec_Mul_Xf_Xf((const char*)l_vec.Points, 3, (const char*)l_vec2.Points, (char*)l_out.Points);
	Vec_Mul_3f_3f(&l_vec, &l_vec2, &l_out);

	return 0;
}