#include "Observer.hpp"
#include "Callback.hpp"
#include "Functional/Equals/Equals.h"

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
		VectorT_eraseCompare(&p_observer->Subjects, ComparatorT<Callback, Callback, void>{ Callback_equals, p_subject, nullptr });
	};

	void Observer_broadcast(Observer* p_observer, void* p_input)
	{
		auto l_it = VectorT_buildIterator(&p_observer->Subjects);
		while (VectorIteratorT_moveNext(&l_it))
		{
			Callback_call(l_it.Current, p_input);
		}
	};

}