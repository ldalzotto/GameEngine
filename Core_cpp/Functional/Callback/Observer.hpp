#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Functional/Callback/Callback.hpp"

namespace _Core
{
	struct Observer
	{
		VectorT<Callback> Subjects;
	};

	void ObserverAlloc(Observer* p_observer);
	void ObserverFree(Observer* p_observer);
	void Observer_register(Observer* p_observer, Callback* p_subject);
	void Observer_unRegister(Observer* p_observer, Callback* p_subject);
	void Observer_broadcast(Observer* p_observer, void* p_input);

}