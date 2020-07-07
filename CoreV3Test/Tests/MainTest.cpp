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

				PushBackArray(&l_vec, (GenericArray*) &l_otherVec);

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

	void CoreTest_Vector()
	{
		CoreTest_Vector_pushBack();
		CoreTest_Vector_insertAt();
		COreTest_Vector_pushBackArray();
	}

	void CoreTest_Execute()
	{
		CoreTest_Vector();
	}
}

