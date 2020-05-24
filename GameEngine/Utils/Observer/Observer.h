#pragma once

#include <vector>

namespace _GameEngine::_Utils
{
	typedef void(*SujectCallback)(void* p_closure, void* p_observerInput);

	struct Subject
	{
		SujectCallback Callback;
		void* Closure;
	};

	struct Observer
	{
		std::vector<Subject*> Subjects;
	};

	void Observer_register(Observer* p_observer, Subject* p_subject);
	void Observer_unRegister(Observer* p_observer, Subject* p_subject);
	void Observer_broadcast(Observer* p_observer, void* p_closure);
};