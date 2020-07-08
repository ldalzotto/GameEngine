#pragma message(__FILE__)

#define ASSERT_EXCEPTION_BEGIN() \
	bool l_errorCatched = false; \
	try \
	{

#define ASSERT_EXCEPTION_END() \
	} \
	catch (const std::exception&) \
	{ \
		l_errorCatched = true; \
	} \
	if (!l_errorCatched) \
	{ \
		throw std::runtime_error("Test fail"); \
	}

namespace _CoreV3
{
	struct Vector3fTest
	{
		float x, y, z;
	};

	template<>
	bool EqualsMethod<Vector3fTest, Vector3fTest, void>(Vector3fTest* p_left, Vector3fTest* p_right, void* p_null)
	{
		return EqualsMethod<float, float, void>(&p_left->x, &p_right->x, NULL)
			&& EqualsMethod<float, float, void>(&p_left->y, &p_right->y, NULL)
			&& EqualsMethod<float, float, void>(&p_left->z, &p_right->z, NULL);
	}

	template<>
	short int SortMethod<Vector3fTest, Vector3fTest, void>(Vector3fTest* p_left, Vector3fTest* p_right, void* p_null)
	{
		return SortMethod<float, float, void>(&p_left->x, &p_right->x, NULL);
	}

	template<class SOURCE_ELEMENT, class COMPARED_ELEMENT>
	void Assert_Equals(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT* p_right)
	{
		if (!_CoreV3::EqualsMethod(p_left, p_right, (void*)NULL))
		{
			throw std::runtime_error("Assert_Equals : fail");
		}
	}

	template<class SOURCE_ELEMENT, class COMPARED_ELEMENT>
	__forceinline void Assert_Equals(SOURCE_ELEMENT* p_left, COMPARED_ELEMENT&& p_right)
	{
		Assert_Equals<SOURCE_ELEMENT, COMPARED_ELEMENT>(p_left, &p_right);
	}

	void CoreTest_Vector_pushBack()
	{
		// PushBack push at last position
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 2);
			{
				PushBack(&l_vec, { 2.0f, 2.0f, 2.0f });
				Assert_Equals(At(&l_vec, 0), Vector3fTest{ 2.0f, 2.0f, 2.0f });
				Assert_Equals(&l_vec.Size, 1);
			}
			Free(&l_vec);
		}

		// PushBack re-allocate vector if not enough space
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 2);
			{
				for (size_t i = 0; i < 2; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
				}

				PushBack(&l_vec, { 2.0f, 2.0f, 2.0f });
				Assert_Equals(At(&l_vec, 2), Vector3fTest{ 2.0f, 2.0f, 2.0f });
				Assert_Equals(&l_vec.Size, 3);
			}
			Free(&l_vec);
		}
	}

	void COreTest_Vector_pushBackArray()
	{
		// PushBackArray pushes another datastructure
		{
			VectorT<Vector3fTest> l_vec;
			VectorT<Vector3fTest> l_otherVec;
			Alloc(&l_vec, 2);
			Alloc(&l_otherVec, 2);
			{
				for (size_t i = 0; i < 2; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
					PushBack(&l_otherVec, { (float)(i + 1), (float)(i + 1), (float)(i + 1) });
				}

				PushBackArray(&l_vec, &l_otherVec);

				Assert_Equals(&l_vec.Size, 4);

				for (size_t i = 0; i < 2; i++)
				{
					Assert_Equals(At(&l_vec, 2 + i), Vector3fTest{ (float)(i + 1), (float)(i + 1), (float)(i + 1) });
				}
			}
			Free(&l_vec);
		}
	}

	void CoreTest_Vector_insertAt()
	{
		// InsertAt copy element at the desired index
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
				}

				InsertAt(&l_vec, { 10.0f, 10.0f, 10.0f }, 1);
				Assert_Equals(At(&l_vec, 1), Vector3fTest{ 10.0f, 10.0f, 10.0f });
			}
			Free(&l_vec);
		}

		// InsertAt throw exception if the index is out ofr range
		{
			ASSERT_EXCEPTION_BEGIN();
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 3);
			{
				InsertAt(&l_vec, { 10.0f, 10.0f, 10.0f }, 4);
			}
			Free(&l_vec);
			ASSERT_EXCEPTION_END();
		}
	}

	void CoreTest_Vector_insertArrayAt()
	{
		// InsertArrayAt copy another datastructure at the desired index
		{
			VectorT<Vector3fTest> l_vec, l_vec2;
			Alloc(&l_vec, 3); Alloc(&l_vec2, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
					PushBack(&l_vec2, { (float)i, (float)i , (float)i });
				}

				InsertArrayAt(&l_vec, &l_vec2, 1);

				for (size_t i = 1; i < 4; i++)
				{
					Assert_Equals(At(&l_vec, i), Vector3fTest{ (float)(i - 1), (float)(i - 1), (float)(i - 1) });
				}

			}
			Free(&l_vec); Free(&l_vec2);
		}

		// InsertArrayAt throw exception if index is out of range
		{
			ASSERT_EXCEPTION_BEGIN();
			VectorT<Vector3fTest> l_vec, l_vec2;
			Alloc(&l_vec, 3); Alloc(&l_vec2, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
					PushBack(&l_vec2, { (float)i, (float)i , (float)i });
				}

				InsertArrayAt(&l_vec, &l_vec2, 4);
			}
			Free(&l_vec); Free(&l_vec2);
			ASSERT_EXCEPTION_END();
		}
	}

	void CoreTest_Vector_swap()
	{
		// Swap swap container element
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
				}

				Swap(&l_vec, 0, 1);
				Assert_Equals(At(&l_vec, 0), Vector3fTest{ 1.0f, 1.0f, 1.0f });
			}
			Free(&l_vec);
		}

		// Swap throw exception if index are out of bound
		{
			ASSERT_EXCEPTION_BEGIN();
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
				}

				Swap(&l_vec, 0, 60);
				Assert_Equals(At(&l_vec, 0), Vector3fTest{ 1.0f, 1.0f, 1.0f });
			}
			Free(&l_vec);
			ASSERT_EXCEPTION_END();
		}

		// Swap throw exception if index are out of bound
		{
			ASSERT_EXCEPTION_BEGIN();
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 3);
			{
				for (size_t i = 0; i < 3; i++)
				{
					PushBack(&l_vec, { (float)i, (float)i , (float)i });
				}

				Swap(&l_vec, 60, 0);
				Assert_Equals(At(&l_vec, 0), Vector3fTest{ 1.0f, 1.0f, 1.0f });
			}
			Free(&l_vec);
			ASSERT_EXCEPTION_END();
		}
	}

	void CoreTest_Vector_sort()
	{
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 4);
			{
				PushBack(&l_vec, { 2.0f, 2.0f, 2.0f });
				PushBack(&l_vec, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_vec, { 1.0f, 1.0f, 1.0f });
				PushBack(&l_vec, { 3.0f, 3.0f, 3.0f });

				Sort_selection(&l_vec, ElementSorterT<Vector3fTest, Vector3fTest, void>{ SortMethod<Vector3fTest, Vector3fTest, void>, NULL });

				for (size_t i = 0; i < 4; i++)
				{
					Assert_Equals(At(&l_vec, i), Vector3fTest{ (float)i, (float)i, (float)i });
				}

			}
			Free(&l_vec);
		}

		// Sorting on an empty vector does nothing
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 4);
			{
				Sort_selection(&l_vec, ElementSorterT<Vector3fTest, Vector3fTest, void>{ SortMethod<Vector3fTest, Vector3fTest, void>, NULL });
				Assert_Equals(&l_vec.Size, 0);
			}
			Free(&l_vec);
		}
	}

	void CoreTest_Vector_find()
	{
		{
			VectorT<Vector3fTest> l_vec;
			Alloc(&l_vec, 4);
			{
				PushBack(&l_vec, { 2.0f, 2.0f, 2.0f });
				PushBack(&l_vec, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_vec, { 1.0f, 1.0f, 1.0f });
				PushBack(&l_vec, { 3.0f, 3.0f, 3.0f });

				{
					Vector3fTest l_findElement = { 1.0f, 1.0f, 1.0f };
					Vector3fTest* l_foundElement = Find(&l_vec, ElementComparatorT<Vector3fTest, Vector3fTest, void>{ EqualsMethod<Vector3fTest, Vector3fTest, void>, & l_findElement, NULL });
					Assert_Equals((void**)&l_foundElement, (void*)At(&l_vec, 2));
				}

				// find returns NULL if element not found
				{
					Vector3fTest l_findElement = { 10.0f, 10.0f, 10.0f };
					Vector3fTest* l_foundElement = Find(&l_vec, ElementComparatorT<Vector3fTest, Vector3fTest, void>{ EqualsMethod<Vector3fTest, Vector3fTest, void>, & l_findElement, NULL });
					Assert_Equals((void**)&l_foundElement, (void*)NULL);
				}
			}
			Free(&l_vec);
		}
	}

	void CoreTest_Vector()
	{
		CoreTest_Vector_pushBack();
		COreTest_Vector_pushBackArray();
		CoreTest_Vector_insertAt();
		CoreTest_Vector_insertArrayAt();
		CoreTest_Vector_swap();
		CoreTest_Vector_sort();
		CoreTest_Vector_find();
	}

	void CoreTest_Array_pushBack()
	{
		// PushBack throw exception if array is full
		{
			ArrayT<Vector3fTest> l_arr;
			Alloc(&l_arr, 2);
			{
				PushBack(&l_arr, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_arr, { 1.0f, 1.0f, 1.0f });

				ASSERT_EXCEPTION_BEGIN();
				PushBack(&l_arr, { 1.0f, 1.0f, 1.0f });
				ASSERT_EXCEPTION_END();
			}
			Free(&l_arr);
		}
	}

	void CoreTest_Array_pushBackArray()
	{
		// PushBackArray throw exception if copied array size if too high
		{
			ArrayT<Vector3fTest> l_arr, l_arr2;
			Alloc(&l_arr, 3); Alloc(&l_arr2, 2);
			{
				PushBack(&l_arr, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_arr, { 1.0f, 1.0f, 1.0f });
				PushBack(&l_arr2, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_arr2, { 1.0f, 1.0f, 1.0f });

				ASSERT_EXCEPTION_BEGIN();
				PushBackArray(&l_arr, &l_arr2);
				ASSERT_EXCEPTION_END();
			}
			Free(&l_arr); Free(&l_arr2);
		}
	}

	void CoreTest_Array_insertAt()
	{
		// InserAt throw exception if copied array size if too high
		{
			ArrayT<Vector3fTest> l_arr;
			Alloc(&l_arr, 2);
			{
				PushBack(&l_arr, { 0.0f, 0.0f, 0.0f });
				InsertAt(&l_arr, { 1.0f, 1.0f, 1.0f }, 0);

				Assert_Equals(At(&l_arr, 0), Vector3fTest{ 1.0f, 1.0f, 1.0f });

				ASSERT_EXCEPTION_BEGIN();
				InsertAt(&l_arr, { 1.0f, 1.0f, 1.0f }, 0);
				ASSERT_EXCEPTION_END();
			}
			Free(&l_arr);
		}
	}

	void CoreTest_Array_insertArrayAt()
	{
		// InsertArrayAt throw exception if copied array size if too high
		{
			ArrayT<Vector3fTest> l_arr, l_arr2;
			Alloc(&l_arr, 3); Alloc(&l_arr2, 2);
			{
				PushBack(&l_arr, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_arr, { 1.0f, 1.0f, 1.0f });
				PushBack(&l_arr2, { 0.0f, 0.0f, 0.0f });
				PushBack(&l_arr2, { 1.0f, 1.0f, 1.0f });

				ASSERT_EXCEPTION_BEGIN();
				InsertArrayAt(&l_arr, &l_arr2, 1);
				ASSERT_EXCEPTION_END();
			}
			Free(&l_arr); Free(&l_arr2);
		}
	}

	void CoreTest_Array()
	{
		CoreTest_Array_pushBack();
		CoreTest_Array_pushBackArray();
		CoreTest_Array_insertAt();
		CoreTest_Array_insertArrayAt();
	}

	void CoreTest_SortedVector_pushBack()
	{
		// When pushing back on a sorted vector, elements are automatically sorted
		SortedVectorT<Vector3fTest> l_svec;
		Alloc(&l_svec, 3, SortMethod<Vector3fTest, Vector3fTest, void>);
		{
			PushBack(&l_svec, { 2.0f, 2.0f, 2.0f });
			PushBack(&l_svec, { 0.0f, 0.0f, 0.0f });
			PushBack(&l_svec, { 1.0f, 1.0f, 1.0f });

			for (size_t i = 0; i < 3; i++)
			{
				Assert_Equals(At(&l_svec, i), Vector3fTest{ (float)i, (float)i, (float)i });
			}
		}
		Free(&l_svec);
	}

	void CoreTest_SortedVector()
	{
		CoreTest_SortedVector_pushBack();
	}
	
	void CoreTest_String_pushBack()
	{
		//PushBack on string append one character
		String l_str;
		Alloc(&l_str, 0);
		{
			PushBack(&l_str, STR("T"));
			Assert_Equals(At(&l_str, 0), STR("T"));
			Assert_Equals(&l_str.Size, 2);
		}
		Free(&l_str);
	}

	void CoreTest_String_pushBackArray()
	{
		String l_str;
		Alloc(&l_str, 0);
		{
			PushBackArray(&l_str, STR("Bonjour"));
			Assert_Equals(At(&l_str, 0), STR("B"));
			Assert_Equals(At(&l_str, 1), STR("o"));
			Assert_Equals(At(&l_str, 2), STR("n"));
			Assert_Equals(At(&l_str, 3), STR("j"));
			Assert_Equals(At(&l_str, 4), STR("o"));
			Assert_Equals(At(&l_str, 5), STR("u"));
			Assert_Equals(At(&l_str, 6), STR("r"));

			Assert_Equals(&l_str.Size, 8);

			// We contat another String
			PushBackArray(&l_str, &l_str);
			Assert_Equals(&l_str.Size, 15);
		}
		Free(&l_str);
	}

	void CoreTest_String()
	{
		CoreTest_String_pushBack();
		CoreTest_String_pushBackArray();
	}

	void CoreTest_Execute()
	{
		CoreTest_Vector();
		CoreTest_Array();
		CoreTest_SortedVector();
		CoreTest_String();
	}
}

