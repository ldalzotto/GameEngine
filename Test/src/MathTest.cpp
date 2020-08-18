
extern "C"
{
#include "v2/_interface/VectorStructuresC.h"
}

int main()
{
	ARRAY_VECTOR3F l_vec;
	Arr_Alloc_Vector3F(&l_vec, 10);
	VECTOR3F l_vecO = { 2.0f, 3.0f, 1.0f };
	Arr_PushBackRealloc_Vector3F(&l_vec, &l_vecO);
	l_vecO.y = 10.0f;
	Arr_PushBackRealloc_Vector3F(&l_vec, &l_vecO);

	ARRAY_VECTOR3F_ITERATOR l_it; Arr_BuildIterator_Vector3F(&l_vec, &l_it);
	while (Iter_MoveNext_Vector3F(&l_it))
	{
		l_it.Current->y;
	}

	Arr_Free_Vector3F(&l_vec);
	return 0;
}