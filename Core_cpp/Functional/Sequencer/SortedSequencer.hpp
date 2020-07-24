#pragma once

#include <stdint.h>
#include "Functional/Callback/Callback.hpp"
#include "DataStructures/Specifications/ArrayT.hpp"
#include "DataStructures/Specifications/SortedVectorT.hpp"

namespace _Core
{
	typedef uint16_t SortedSequencerPriority;
	bool SortedSequencerPriority_equals(SortedSequencerPriority* p_left, SortedSequencerPriority* p_right, void* p_null);

	struct SortedSequencerOperation
	{
		SortedSequencerPriority Priority;
		Callback OperationCallback;
	};

	struct SortedSequencer
	{
		SortedVectorT<SortedSequencerOperation> Operations;
	};

	void SortedSequencer_alloc(SortedSequencer* p_sequencer);
	void SortedSequencer_free(SortedSequencer* p_sequencer);

	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation);
	void SortedSequencer_removeOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation);
	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input);
	void SortedSequencer_execute_afterCallback(SortedSequencer* p_sortedSequencer, void* p_input, Callback* p_afterEveryExecutionCallback);

	SortedSequencerPriority SortedSequencer_calculatePriority(ArrayT<SortedSequencerPriority>* p_before, ArrayT<SortedSequencerPriority>* p_after);
}


