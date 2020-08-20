#pragma once

#include "SortedSequencer.hpp"
#include "Functional/Callback/CallbackT.hpp"
#include "DataStructures/Specifications/VectorT.hpp"

namespace _Core
{
	template <typename INPUT_TYPE>
	struct SortedSequencerOperationT
	{
		SortedSequencerPriority Priority;
		CallbackT<void, INPUT_TYPE> OperationCallback;
	};

	template <typename INPUT_TYPE>
	struct SortedSequencerT
	{
		VectorT<SortedSequencerOperationT<INPUT_TYPE>> Operations;
	};

	template <typename INPUT_TYPE>
	inline void SortedSequencerT_alloc(SortedSequencerT<INPUT_TYPE>* p_sequencer)
	{
		SortedSequencer_alloc((SortedSequencer*)p_sequencer);
	};
	
	template <typename INPUT_TYPE>
	inline void SortedSequencerT_free(SortedSequencerT<INPUT_TYPE>* p_sequencer)
	{
		SortedSequencer_free((SortedSequencer*)p_sequencer);
	};

	template <typename INPUT_TYPE>
	inline void SortedSequencerT_addOperation(SortedSequencerT<INPUT_TYPE>* p_sortedSequencer, SortedSequencerOperationT<INPUT_TYPE>* p_sortedSequencerOperation)
	{
		SortedSequencer_addOperation((SortedSequencer*)p_sortedSequencer, (SortedSequencerOperation*)p_sortedSequencerOperation);
	};
	
	template <typename INPUT_TYPE>
	inline void SortedSequencerT_execute(SortedSequencerT<INPUT_TYPE>* p_sortedSequencer, INPUT_TYPE* p_input)
	{
		SortedSequencer_execute((SortedSequencer*)p_sequencer, p_input);
	};
	
	template <typename INPUT_TYPE, typename AFTER_EXECUTION_CLOSURE_TYPE>
	inline void SortedSequencerT_execute_afterCallback(SortedSequencerT<INPUT_TYPE>* p_sortedSequencer, INPUT_TYPE* p_input, CallbackT<AFTER_EXECUTION_CLOSURE_TYPE, void>* p_afterEveryExecutionCallback)
	{
		SortedSequencer_execute_afterCallback((::_Core::SortedSequencer*)p_sortedSequencer, p_input, (Callback_PTR)p_afterEveryExecutionCallback);
	};
}