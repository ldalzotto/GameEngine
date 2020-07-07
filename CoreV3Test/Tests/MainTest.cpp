#pragma message(__FILE__)

struct Vector3fTest
{
	float x, y, z;
};

Vector3fTest getOneingElement()
{
	return { 1.0f, 1.0f, 1.0f };
}

void CoreTest_Execute()
{
	_CoreV3::VectorT<Vector3fTest> l_vec;
	_CoreV3::Alloc(&l_vec, 0);
	for (size_t i = 0; i < 5; i++)
	{
		_CoreV3::PushBack(&l_vec, { (float)i, (float)i, (float)i });
	}
	auto l_zd = _CoreV3::At(&l_vec, 2);
	_CoreV3::Free(&l_vec);
}