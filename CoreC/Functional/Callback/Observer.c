#include "Observer.h"

#include "Functional/Vector/VectorWriter.h"
#include "Functional/Vector/VectorAccessor.h"
#include "Functional/Vector/VectorIterator.h"

void Core_ObserverAlloc(Core_Observer* p_observer)
{
	Core_GenericArray_alloc(&(p_observer->Subjects), sizeof(Callback), 0);
};

void Core_ObserverFree(Core_Observer* p_observer)
{
	Core_GenericArray_free(&p_observer->Subjects);
};

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