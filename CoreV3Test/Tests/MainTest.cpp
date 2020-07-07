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
	return p_left->x == p_right->x;
}

short int Compare(Vector3fTest* p_left, Vector3fTest* p_right, void* p_null)
{
	if (p_left->x > p_right->x) { return 1; }
	else if (p_left->x < p_right->x) { return -1; }
	else { return 0; }
}

void CoreTest_Execute()
{
	_CoreV3::VectorT<Vector3fTest> l_vec;
	_CoreV3::Alloc(&l_vec, 0);
	for (size_t i = 0; i < 5; i++)
	{
		_CoreV3::InsertAt(&l_vec, { (float)i, (float)i, (float)i }, 0);
		// _CoreV3::PushBack(&l_vec, { (float)i, (float)i, (float)i });
	}
	auto l_zd = _CoreV3::At(&l_vec, 2);
	
	Vector3fTest* l_foundElement = _CoreV3::Find(&l_vec, _CoreV3::ElementComparatorT<Vector3fTest, Vector3fTest, void> {Equals, l_zd, NULL});
	_CoreV3::Sort_selection(&l_vec, _CoreV3::ElementSorterT<Vector3fTest, Vector3fTest, void>{ Compare, NULL });
	_CoreV3::Free(&l_vec);


}