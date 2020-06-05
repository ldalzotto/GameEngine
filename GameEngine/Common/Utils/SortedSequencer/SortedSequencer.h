#pragma once

#include <vector>

namespace _GameEngine::_Utils
{

	typedef uint16_t SortedSequencerPriority;

	struct SortedSequencerOperation
	{
		SortedSequencerPriority Priority;
		void* Closure;
		void(*Callback)(void* p_closure, void* p_input);
	};

	struct SortedSequencer
	{
		std::vector<SortedSequencerOperation> Operations;
	};

	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation);
	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input);

	SortedSequencerPriority SortedSequencer_calculatePriority(std::vector<SortedSequencerPriority>* p_before, std::vector<SortedSequencerPriority>* p_after);
}