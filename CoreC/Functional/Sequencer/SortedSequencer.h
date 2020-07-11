#pragma once

#include "DataStructures/GenericArray.h"
#include "DataStructures/Specifications/SortedVector.h"
#include "DataStructures/GenericArrayNameMacros.h"
#include "Functional/Callback/Callback.h"

typedef unsigned __int16 SortedSequencerPriority;

typedef Core_GenericArray CORE_VECTOR_NAME(SortedSequencerPriority);

typedef struct Core_SortedSequencerOperation
{
	SortedSequencerPriority Priority;
	Callback OperationCallback;
} Core_SortedSequencerOperation;

typedef Core_SortedVector Core_SortedVector_SortedSequencerOperation;

typedef struct Core_SortedSequencer
{
	Core_SortedVector_SortedSequencerOperation Operations;
} Core_SortedSequencer;

void Core_SortedSequencer_alloc(Core_SortedSequencer* p_sequencer);
void Core_SortedSequencer_free(Core_SortedSequencer* p_sequencer);

void Core_SortedSequencer_addOperation(Core_SortedSequencer* p_sortedSequencer, Core_SortedSequencerOperation* p_sortedSequencerOperation);
void Core_SortedSequencer_execute(Core_SortedSequencer* p_sortedSequencer, void* p_input);
void Core_SortedSequencer_execute_afterCallback(Core_SortedSequencer* p_sortedSequencer, void* p_input, Callback p_afterEveryExecutionCallback);

SortedSequencerPriority Core_SortedSequencer_calculatePriority(CORE_VECTOR_NAME(SortedSequencerPriority)* p_before, CORE_VECTOR_NAME(SortedSequencerPriority)* p_after);