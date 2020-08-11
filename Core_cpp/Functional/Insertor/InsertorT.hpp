#pragma once

namespace _Core
{
	struct GenericArray;
}

namespace _Core
{
	template <typename T>
	using InsertorT_PushBackFn = T * (*)(GenericArray*, T*);

	template<typename T>
	struct InsertorT
	{
		GenericArray* Array;
		InsertorT_PushBackFn<T> PushBackFunction;
	};

	template<typename T>
	T* InsertorT_pushBack(InsertorT<T>* p_insert, T* p_value)
	{
		return p_insert->PushBackFunction(p_insert->Array, p_value);
	};
}