#pragma once

#include <stdint.h>
#include "DataStructures/VectorT.h"

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
		_Core::SortedVectorT<SortedSequencerOperation> OperationsV2;
	};

	void SortedSequencer_alloc(SortedSequencer* p_sortedSequencer);
	void SortedSequender_free(SortedSequencer* p_sortedSequencer);

	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation);
	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input);

	SortedSequencerPriority SortedSequencer_calculatePriority(std::vector<SortedSequencerPriority>* p_before, std::vector<SortedSequencerPriority>* p_after);
}