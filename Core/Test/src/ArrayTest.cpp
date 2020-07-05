#include "ArrayTest.h"

extern "C" {
	#include "DataContainer/Structures/Vector.h"
	#include "DataContainer/Structures/SortedVector.h"
	#include "DataContainer/FunctionalObjets/VectorIterator.h"
	#include "DataContainer/Algorithm/SortingAlgorithm.h"
	#include "DataContainer/Algorithm/SortingAlgorithmMacro.h"
	#include "Error/ErrorHandling.h"
}

namespace _Core::_Test
{
	CORE_DEFINE_VECTOR_IMPL(Vector3f_Test);
	CORE_VECTORWRITER_DEFINE_IMPL(Core_Vector3f_Test_Vector, Vector3f_Test);
	CORE_ALGORITHM_DEFINE_IMPL(Core_Vector3f_Test_Vector, Vector3f_Test);

	CORE_DEFINE_SORTEDVECTOR_IMPL(Vector3f_Test);
	CORE_VECTORWRITER_DEFINE_IMPL(Core_Vector3f_Test_SortedVector, Vector3f_Test);
	CORE_ALGORITHM_DEFINE_IMPL(Core_Vector3f_Test_SortedVector, Vector3f_Test);

	short int Vector3f_Test_SortComparator_testV2(Vector3f_Test* p_left, Vector3f_Test* p_right, void* p_null)
	{
		if (p_left->x > p_right->x) { return -1; }
		else if (p_left->x < p_right->x) { return 1; }
		else { return 0; }
	}

	void ExecuteTest()
	{
		Core_Vector3f_Test_Vector l_vec3V;
		Core_Vector3f_Test_Vector_alloc(&l_vec3V, 10);
		{
			for (size_t i = 0; i < 11; i++)
			{
				Vector3f_Test l_value = { (float)i, (float)i, (float)i };
				Core_Vector3f_Test_VectorWriter_pushBack(&l_vec3V.Writer, &l_value);
			}

			CORE_VECTORITERATOR_FOREACH_BEGIN(Core_Vector3f_Test_Vector, &l_vec3V, l_it)
				l_it.Current->x += 1;
			CORE_VECTORITERATOR_FOREACH_END();

			CORE_ALGO_MIN_IT(Core_Vector3f_Test_Vector, Vector3f_Test, &l_vec3V, l_min, Vector3f_Test_SortComparator_testV2);
			CORE_ALGO_SELECTION_SORT_IT(Core_Vector3f_Test_Vector, Vector3f_Test, &l_vec3V, Vector3f_Test_SortComparator_testV2);
		}
		Core_Vector3f_Test_Vector_free(&l_vec3V);


		Core_Vector3f_Test_SortedVector l_vec3VSorted;
		Core_Vector3f_Test_SortedVector_alloc(&l_vec3VSorted, 10, Vector3f_Test_SortComparator_testV2);
		{
			for (size_t i = 0; i < 11; i++)
			{
				Vector3f_Test l_value = { (float)i, (float)i, (float)i };
				Core_Vector3f_Test_SortedVectorWriter_pushBack(&l_vec3VSorted.Writer, &l_value);
			}

			CORE_VECTORITERATOR_FOREACH_BEGIN(Core_Vector3f_Test_SortedVector, &l_vec3VSorted, l_it)
				l_it.Current->x += 1;
			CORE_VECTORITERATOR_FOREACH_END();

			CORE_ALGO_MIN_IT(Core_Vector3f_Test_SortedVector, Vector3f_Test, &l_vec3VSorted, l_min, Vector3f_Test_SortComparator_testV2);
			CORE_ALGO_SELECTION_SORT_IT(Core_Vector3f_Test_SortedVector, Vector3f_Test, &l_vec3VSorted, Vector3f_Test_SortComparator_testV2);
		}
		Core_Vector3f_Test_SortedVector_free(&l_vec3VSorted);
	};
}