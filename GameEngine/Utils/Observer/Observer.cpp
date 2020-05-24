#include "Observer.h"

namespace _GameEngine::_Utils
{
	void Observer_register(Observer* p_observer, Subject* p_subject)
	{
		p_observer->Subjects.push_back(p_subject);
	};

	void Observer_unRegister(Observer* p_observer, Subject* p_subject)
	{
		for (uint32_t i = 0; i < p_observer->Subjects.size(); i++)
		{
			if (p_observer->Subjects[i] == p_subject)
			{
				p_observer->Subjects.erase(p_observer->Subjects.begin() + i);
				return;
			}
		}
	};

	void Observer_broadcast(Observer* p_observer, void* p_input)
	{
		for (auto&& l_subject : p_observer->Subjects)
		{
			l_subject->Callback(l_subject->Closure, p_input);
		}
	};
};