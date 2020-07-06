#pragma message(__FILE__)

typedef struct CoreTest_Vector3f
{
	float x, y, z;
} CoreTest_Vector3f;

CORE_VECTORACCESSOR_DEFINE(Core_CoreTest_Vector3f_Vector, CoreTest_Vector3f);
CORE_VECTORWRITER_DEFINE(Core_CoreTest_Vector3f_Vector, CoreTest_Vector3f);
CORE_DEFINE_VECTOR(CoreTest_Vector3f);
CORE_VECTORITERATOR_DEFINE(Core_CoreTest_Vector3f_Vector, CoreTest_Vector3f);
CORE_DEFINE_VECTOR_IMPL(CoreTest_Vector3f);

/*
CORE_VECTORACCESSOR_DEFINE(Core_CoreTest_Vector3f_Array, CoreTest_Vector3f);
CORE_VECTORWRITER_DEFINE(Core_CoreTest_Vector3f_Array, CoreTest_Vector3f);
CORE_DEFINE_ARRAY(CoreTest_Vector3f);
CORE_VECTORITERATOR_DEFINE(Core_CoreTest_Vector3f_Array, CoreTest_Vector3f);
CORE_DEFINE_ARRAY_IMPL(CoreTest_Vector3f);
*/

CORETEST_BEGIN(CoreTest_Writer_pushBack_addsAnElement)
{
	CORE_VECTOR_ALLOC(CoreTest_Vector3f, 5, l_vec)
	{
		CoreTest_Vector3f l_element = { 1.0f, 1.0f, 1.0f };
		CORE_VECTORWRITER_PUSHBACK(&l_vec, &l_element);
		CORETEST_ASSERT_EQUALS(l_element.x, CORE_VECTORACCESSOR_AT(&l_vec, 0)->x);
		CORETEST_ASSERT_EQUALS(1, l_vec.Size);
	}
	CORE_VECTOR_FREE(CoreTest_Vector3f, &l_vec);
}
CORETEST_END();

/*
	When we try to push elements in a vector, it's memory is reallocated if there is not enough space.
*/
CORETEST_BEGIN(CoreTest_Writer_pushBack_reallocation)
{
	CORE_VECTOR_ALLOC(CoreTest_Vector3f, 0, l_vec);
	{
		{
			CoreTest_Vector3f l_element = { 0.0f, 0.0f, 0.0f };
			CORE_VECTORWRITER_PUSHBACK(&l_vec, &l_element);
			CORETEST_ASSERT_EQUALS(l_element.x, CORE_VECTORACCESSOR_AT(&l_vec, 0)->x);
			CORETEST_ASSERT_EQUALS(1, l_vec.Size);
		}

		for (size_t i = 1; i < 5; i++)
		{
			CoreTest_Vector3f l_element = { (float)i, (float)i, (float)i };
			CORE_VECTORWRITER_PUSHBACK(&l_vec, &l_element);
		}

		CORETEST_ASSERT_EQUALS(5, l_vec.Size);
		CORETEST_ASSERT_SUPERIOR_STRICT(l_vec.Capacity, 5);
	}
	CORE_VECTOR_FREE(CoreTest_Vector3f, &l_vec);
}
CORETEST_END()

CORETEST_BEGIN(CoreTest_Writer_insertAt)
{
	CORE_VECTOR_ALLOC(CoreTest_Vector3f, 4, l_vec);
	{
		// Vector initialization
		for (size_t i = 0; i < 3; i++)
		{
			CoreTest_Vector3f l_element = { 1.0f, 1.0f, 1.0f };
			CORE_VECTORWRITER_INSERT_AT(&l_vec, &l_element, 1, 0);
		}

		// We insert at index 1
		{
			CoreTest_Vector3f l_element = { 2.0f, 2.0f, 2.0f };
			CORE_VECTORWRITER_INSERT_AT(&l_vec, &l_element, 1, 1);
			CORETEST_ASSERT_EQUALS(l_element.x, CORE_VECTORACCESSOR_AT(&l_vec, 1)->x);
		}

		// An error is raised when we try to insert outside of vector size
		{
			CORETEST_ASSERT_ERROR_BEGIN();
			{
				CoreTest_Vector3f l_element = { 2.0f, 2.0f, 2.0f };
				CORE_VECTORWRITER_INSERT_AT(&l_vec, &l_element, 1, l_vec.Capacity + 1);
			}
			CORETEST_ASSERT_ERROR_END(CR_OUT_OF_BOUND)
		}
	}
	CORE_VECTOR_FREE(CoreTest_Vector3f, &l_vec);
}
CORETEST_END()

void CoreTest_WriterTests()
{
	CoreTest_Writer_pushBack_addsAnElement();
	CoreTest_Writer_pushBack_reallocation();
	CoreTest_Writer_insertAt();
}