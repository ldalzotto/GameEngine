#pragma once

extern "C"
{
#include "Functional/Callback/Observer.h"
}

#include "DataStructures/Specifications/VectorT.hpp"
#include "CallbackT.hpp"

namespace _Core
{
	template <typename INPUT_TYPE>
	struct ObserverT
	{
		VectorT<CallbackT<void, INPUT_TYPE>*> Subjects;
	};

	template <typename INPUT_TYPE>
	inline void ObserverT_alloc(ObserverT<INPUT_TYPE>* p_observer)
	{
		Observer_Alloc((Observer*)p_observer);
	};
	
	template <typename INPUT_TYPE>
	inline void ObserverT_free(ObserverT<INPUT_TYPE>* p_observer)
	{
		Observer_Free((Observer*)p_observer);
	};
	
	template <typename INPUT_TYPE>
	inline void ObserverT_register(ObserverT<INPUT_TYPE>* p_observer, CallbackT<void, INPUT_TYPE>* p_subject)
	{
		Observer_Register((Observer*)p_observer, (Callback*)p_subject);
	};
	
	template <typename INPUT_TYPE>
	inline void ObserverT_unRegister(ObserverT<INPUT_TYPE>* p_observer, CallbackT<void, INPUT_TYPE>* p_subject)
	{
		Observer_UnRegister((Observer*)p_observer, (Callback*)p_subject);
	};
	
	template <typename INPUT_TYPE>
	inline void ObserverT_broadcast(ObserverT<INPUT_TYPE>* p_observer, INPUT_TYPE* p_input)
	{
		Observer_Broadcast((Observer*)p_observer, p_input);
	};
}