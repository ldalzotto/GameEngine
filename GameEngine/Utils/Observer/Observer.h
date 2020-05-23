#pragma once

#include <vector>
#include <functional>

namespace _GameEngine::_Utils::_Observer
{
	struct Subject
	{
		std::function<void(void*)> Callback;
	};

	struct Observer
	{
		std::vector<Subject> Subjects;
	};

	Subject* Observer_register(Observer* p_observer, std::function<void(void*)> p_callback);
	void Observer_unRegister(Observer* p_observer, Subject* p_subject);
	void Observer_broadcast(Observer* p_observer, void* p_closure);
};