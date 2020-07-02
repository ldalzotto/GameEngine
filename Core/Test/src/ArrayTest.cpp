#include "ArrayTest.h"

extern "C" {
	#include "DataContainer/Structures/Vector.h"
	#include "DataContainer/FunctionalObjets/VectorIterator.h"
}

namespace _Core::_Test
{
	CORE_VECTORWRITER_DEFINE_IMPL(Core_Vector3f_Test_Vector, Vector3f_Test);

	void ExecuteTest()
	{
		Core_Vector3f_Test_Vector l_vec3V;
		Core_Vector_alloc((Core_Vector*)&l_vec3V, 10);
		{
			for (size_t i = 0; i < 11; i++)
			{
				Vector3f_Test l_value = { i, i, i };
				Core_Vector3f_Test_VectorWriter_pushBack(&l_vec3V.Writer, &l_value);
			}

			CORE_VECTORITERATOR_FOREACH_BEGIN(Vector3f_Test, &l_vec3V, l_it)
				l_it.Current->x += 1;
			CORE_VECTORITERATOR_FOREACH_END();
		}
		Core_Vector_free((Core_Vector*)&l_vec3V);
	};
}