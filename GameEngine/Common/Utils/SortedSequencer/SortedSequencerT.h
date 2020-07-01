#pragma once

#include <functional>

namespace _GameEngine::_Utils
{
	template<typename SortedSequencerPriorityProviderFn>
	inline _Utils::SortedSequencerPriority SortedSequencer_calculatePriorityT_b1(SortedSequencerPriorityProviderFn p_before1)
	{
		_Core::VectorT<_Utils::SortedSequencerPriority> l_before;
		l_before.alloc(1);
		{
			auto l_index = p_before1();
			l_before.push_back(&l_index);
		}
		_Utils::SortedSequencerPriority l_priority = _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
		l_before.free();
		return std::move(l_priority);
	};

	template<typename SortedSequencerPriorityProviderFn>
	inline _Utils::SortedSequencerPriority SortedSequencer_calculatePriorityT_b2(SortedSequencerPriorityProviderFn p_before1, SortedSequencerPriorityProviderFn p_before2)
	{
		_Core::VectorT<_Utils::SortedSequencerPriority> l_before;
		l_before.alloc(2);
		{
			auto l_index = p_before1();
			l_before.push_back(&l_index);
		}
		{
			auto l_index = p_before2();
			l_before.push_back(&l_index);
		}
		_Utils::SortedSequencerPriority l_priority = _Utils::SortedSequencer_calculatePriority(&l_before, nullptr);
		l_before.free();
		return std::move(l_priority);
	};
}