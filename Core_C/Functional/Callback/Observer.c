#include "Observer.h"

void Observer_Alloc(Observer_PTR p_observer) 
{
	Arr_Alloc_Callback(&p_observer->Subjects, 0);
};

void Observer_Free(Observer_PTR p_observer)
{
	Arr_Free_Callback(&p_observer->Subjects);
};

void Observer_Register(Observer_PTR p_observer, Callback_PTR p_subject)
{
	Arr_PushBackRealloc_Callback(&p_observer->Subjects, p_subject);
};

void Observer_UnRegister(Observer_PTR p_observer, Callback_PTR p_subject)
{
	for (size_t i = 0; i < p_observer->Subjects.Size; i++)
	{
		if (Callback_equals(p_subject, &p_observer->Subjects.Memory[i]))
		{
			Arr_Erase_Callback(&p_observer->Subjects, i);
			return;
		}
	}
};

void Observer_Broadcast(Observer_PTR p_observer, void* p_input)
{
	for (size_t i = 0; i < p_observer->Subjects.Size; i++)
	{
		Callback_call(&p_observer->Subjects.Memory[i], p_input);
	}
};

