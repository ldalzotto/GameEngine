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
	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input, void(*p_afterEveryExecution)(void*), void* p_afterEveryExecution_userData);

	SortedSequencerPriority SortedSequencer_calculatePriority(_Core::VectorT<SortedSequencerPriority>* p_before, _Core::VectorT<SortedSequencerPriority>* p_after);
}
