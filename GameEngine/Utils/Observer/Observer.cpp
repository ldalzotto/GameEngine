#include "Observer.h"

namespace _GameEngine::_Utils
{
	Subject* Observer_register(Observer* p_observer, std::function<void(void*)> p_callback)
	{
		Subject l_subject{};
		l_subject.Callback = p_callback;
		p_observer->Subjects.push_back(l_subject);
		return &p_observer->Subjects.back();
	};

	void Observer_unRegister(Observer* p_observer, Subject* p_subject)
	{
		for (uint32_t i = 0; i < p_observer->Subjects.size(); i++)
		{
			if (&p_observer->Subjects[i] == p_subject)
			{
				p_observer->Subjects.erase(p_observer->Subjects.begin() + i);
				return;
			}
		}
	};

	void Observer_broadcast(Observer* p_observer, void* p_closure)
	{
		for (auto&& l_subject : p_observer->Subjects)
		{
			l_subject.Callback(p_closure);
		}
	};
};