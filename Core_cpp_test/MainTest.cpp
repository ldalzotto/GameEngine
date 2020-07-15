#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/String.hpp"
#include "Functional/Comparator/ComparatorT.hpp"

#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "Algorithm/Sort/SortAlgorithmT.hpp"
#include "DataStructures/Specifications/SortedVectorT.hpp"
#include "DataStructures/Specifications/LinearMapT.hpp"
#include "DataStructures/Specifications/SortedLinearMapT.hpp"

#include "Functional/Equals/Equals.hpp"
#include "Functional/Sort/ElementSorter.hpp"

using namespace _Core;

struct Vector3f
{
	float x, y, z;
};

bool Vector3f_equals(Vector3f* p_left, Vector3f* p_right, void* p_null)
{
	return _Core::Equals_float(&p_left->x, &p_right->x) 
		&& _Core::Equals_float(&p_left->y, &p_right->y)
		&& _Core::Equals_float(&p_left->z, &p_right->z);
}

short int Vector3f_sort(Vector3f* p_left, Vector3f* p_right, void* p_null)
{
	return _Core::SortCompare_float_float(&p_left->x, &p_right->x);
}

_Core::AsserterT<Vector3f, Vector3f, void> Vector3f_EqualsAsserter =
{
	Vector3f_equals, 
	nullptr
};

_Core::ElementSorterT<Vector3f, Vector3f, void> Vector3f_Sorter =
{
	Vector3f_sort,
	nullptr
};

int main()
{
	{
		_Core::VectorT<Vector3f> l_arr;
		_Core::VectorT_alloc(&l_arr, 10);

		{
			_Core::VectorT_pushBack(&l_arr, Vector3f { 1.0f, 1.0f, 1.0f });
		}

		{
			_Core::VectorT_pushBack(&l_arr, Vector3f { -1.0f, -1.0f, -1.0f });
		}

		{
			Vector3f l_comparedValue = { 1.0f, 1.0f, 1.0f };

			ComparatorT<Vector3f, Vector3f, void> l_findVector3f = { Vector3f_equals, &l_comparedValue, (void*)nullptr };
			VectorIteratorT<Vector3f> l_found = CompareT_find(VectorT_buildIterator(&l_arr), l_findVector3f);
			int as = 0;
		}

		{
			VectorIteratorT<Vector3f> l_it = SortT_selectionSort(VectorT_buildIterator(&l_arr), ElementSorterT<Vector3f, Vector3f, void>{Vector3f_sort, nullptr});
		}


		_Core::VectorT_free(&l_arr);
	}

	{
		_Core::SortedVectorT<Vector3f> l_vec;
		_Core::SortedVectorT_alloc(&l_vec, 10);
		{
			Vector3f l_value = { 5.0f, 5.0f, 5.0f };
			_Core::SortedVectorT_pushBack(&l_vec, &l_value, Vector3f_sort);
		}
		{
			Vector3f l_value = { 1.0f, 1.0f, 1.0f };
			_Core::SortedVectorT_pushBack(&l_vec, &l_value, Vector3f_sort);
		}
		_Core::SortedVectorT_free(&l_vec);
	}

	{
		LinearMapT<Vector3f> l_map;
		LinearMapT_alloc(&l_map, 0);
		{
			LinearMapT_pushBack(&l_map, Vector3f { 1.0f, 1.0f, 1.0f }, Vector3f_EqualsAsserter);
			LinearMapT_pushBack(&l_map, Vector3f { 3.0f, 1.0f, 1.0f }, Vector3f_EqualsAsserter);
		}
		LinearMapT_free(&l_map);
	}

	{
		SortedLinearMapT<Vector3f> l_map;
		SortedLinearMapT_alloc(&l_map, 0);
		{
			SortedLinearMapT_pushBack(&l_map, Vector3f{ 3.0f, 1.0f, 1.0f }, Vector3f_EqualsAsserter, Vector3f_Sorter);
			SortedLinearMapT_pushBack(&l_map, Vector3f{ 1.0f, 1.0f, 1.0f }, Vector3f_EqualsAsserter, Vector3f_Sorter);
			SortedLinearMapT_pushBack(&l_map, Vector3f{ 3.0f, 0.0f, 1.0f }, Vector3f_EqualsAsserter, Vector3f_Sorter);
		}
		SortedLinearMapT_free(&l_map);
	}

}