#pragma once

#include "DataContainer/Structures/VectorMacros.h"
#include "DataContainer/FunctionalObjets/VectorIteratorMacros.h"
#include "DataContainer/FunctionalObjets/VectorWriterMacros.h"

namespace _Core::_Test
{

	struct Vector3f_Test
	{
		float x, y, z;
	};

	CORE_VECTORWRITER_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);
	CORE_DEFINE_VECTOR(Vector3f_Test);
	CORE_VECTORITERATOR_DEFINE(Core_Vector3f_Test_Vector, Vector3f_Test);


	void ExecuteTest();
}