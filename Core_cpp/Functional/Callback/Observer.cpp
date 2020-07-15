#include "Observer.hpp"
#include "Callback.hpp"
#include "Functional/Equals/Equals.hpp"

namespace _Core
{
	void ObserverAlloc(Observer* p_observer)
	{
		VectorT_alloc(&p_observer->Subjects, 0);
	};

	void ObserverFree(Observer* p_observer)
	{
		VectorT_free(&p_observer->Subjects);
	};

	void Observer_register(Observer* p_observer, Callback* p_subject)
	{
		VectorT_pushBack(&p_observer->Subjects, p_subject);
	};

	void Observer_unRegister(Observer* p_observer, Callback* p_subject)
	{
		VectorT_eraseCompare(&p_observer->Subjects, ComparatorT<Callback*, Callback*, void>{ CallbackP_equals, &p_subject, nullptr });
	};

	void Observer_broadcast(Observer* p_observer, void* p_input)
	{
		auto l_it = VectorT_buildIterator(&p_observer->Subjects);
		while (VectorIteratorT_moveNext(&l_it))
		{
			Callback_call(*l_it.Current, p_input);
		}
	};

}


/*
void Core_Observer_register(Core_Observer* p_observer, Callback* p_subject)
{
	Core_GenericArray_pushBack_realloc(&p_observer->Subjects, p_subject);
};

void Core_Observer_unRegister(Core_Observer* p_observer, Callback* p_subject)
{
	Core_VectorIterator l_it = Core_GenericArray_buildIterator(&p_observer->Subjects);
	while (Core_VectorIterator_moveNext(&l_it))
	{
		if (Core_GenericArray_at(&p_observer->Subjects, l_it.CurrentIndex) == p_subject)
		{
			Core_GenericArray_erase(&p_observer->Subjects, l_it.CurrentIndex);
			return;
		}
	}
};

void Core_Observer_broadcast(Core_Observer* p_observer, void* p_input)
{
	Core_VectorIterator l_it = Core_GenericArray_buildIterator(&p_observer->Subjects);
	while (Core_VectorIterator_moveNext(&l_it))
	{
		Callback* l_callback = l_it.Current;
		Core_Callback_call(l_callback, p_input);
	}
};
*/