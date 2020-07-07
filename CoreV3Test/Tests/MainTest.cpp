#pragma message(__FILE__)

struct Vector3fTest
{
	float x, y, z;
};

Vector3fTest getOneingElement()
{
	return { 1.0f, 1.0f, 1.0f };
}

bool Equals(Vector3fTest* p_left, Vector3fTest* p_right, void* p_null)
{
	return _CoreV3::EqualsMethod<float, float, void>(&p_left->x, &p_right->x, NULL);
}

short int Compare(Vector3fTest* p_left, Vector3fTest* p_right, void* p_null)
{
	return _CoreV3::SortMethod<float, float, void>(&p_left->x, &p_right->x, NULL);
}

void CoreTest_Execute()
{
	{
		_CoreV3::VectorT<float> l_vec;
		_CoreV3::Alloc(&l_vec, 5);
		for (size_t i = 0; i < 4; i++)
		{
			_CoreV3::InsertAt(&l_vec, (float)i, 0);
		}
		
		_CoreV3::Sort_selection(&l_vec, _CoreV3::ElementSorterT<float, float, void>{ _CoreV3::SortMethod<float, float, void>, NULL });
		_CoreV3::Free(&l_vec);
	}

	{
		_CoreV3::VectorT<Vector3fTest> l_vec;
		_CoreV3::Alloc(&l_vec, 0);
		for (size_t i = 0; i < 5; i++)
		{
			_CoreV3::InsertAt(&l_vec, { (float)i, (float)i, (float)i }, 0);
		}
		auto l_zd = _CoreV3::At(&l_vec, 2);

		Vector3fTest* l_foundElement = _CoreV3::Find(&l_vec, _CoreV3::ElementComparatorT<Vector3fTest, Vector3fTest, void> {Equals, l_zd, NULL});
		_CoreV3::ElementSorterT<Vector3fTest, Vector3fTest, void> l_minComp{ Compare, NULL };
		_CoreV3::Min(&l_vec, &l_minComp);
		_CoreV3::Sort_selection(&l_vec, _CoreV3::ElementSorterT<Vector3fTest, Vector3fTest, void>{ Compare, NULL });
		_CoreV3::Free(&l_vec);

	}

	{
		_CoreV3::SortedVectorT<Vector3fTest> l_vec;
		_CoreV3::Alloc(&l_vec, 0, Compare);

		for (size_t i = 5; i > 0; --i)
		{
			_CoreV3::PushBack(&l_vec, { (float)i, (float)i, (float)i });
		}

		_CoreV3::Free(&l_vec);
	}

	{
		_CoreV3::String l_str;
		_CoreV3::Alloc(&l_str, 3);
		_CoreV3::PushBackArray(&l_str, STR("Test"));
		_CoreV3::PushBack(&l_str, STR("C"));
		_CoreV3::Free(&l_str);
	}


}