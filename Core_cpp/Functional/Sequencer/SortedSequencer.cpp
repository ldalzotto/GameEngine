#include "SortedSequencer.hpp"

#include <stdexcept>

#include "Functional/Sort/ElementSorter.hpp"
#include "Algorithm/Compare/CompareAlgorithmT.hpp"
#include "Functional/Equals/Equals.hpp"

namespace _Core
{
	bool SortedSequencerPriority_equals(SortedSequencerPriority* p_left, SortedSequencerPriority* p_right, void* p_null)
	{
		return Equals_int16t((int16_t*)p_left, (int16_t*)p_right);
	};

	short int SortedSequencerOperation_elementSorter(SortedSequencerOperation* p_left, SortedSequencerOperation* p_right, void* p_null)
	{
		return SortCompare_uint16_uint16(&p_left->Priority, &p_right->Priority);
	}

	void SortedSequencer_alloc(SortedSequencer* p_sequencer)
	{
		SortedVectorT_alloc(&p_sequencer->Operations, 10);
	};

	void SortedSequencer_free(SortedSequencer* p_sequencer)
	{
		SortedVectorT_free(&p_sequencer->Operations);
	};

	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation)
	{
		SortedVectorT_pushBack(&p_sortedSequencer->Operations, p_sortedSequencerOperation, SortedSequencerOperation_elementSorter);
	};

	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input)
	{
		auto l_it = SortedVectorT_buildIterator(&p_sortedSequencer->Operations);
		while (VectorIteratorT_moveNext(&l_it))
		{
			Callback_call(&l_it.Current->OperationCallback, p_input);
		}
	}
	void SortedSequencer_execute_afterCallback(SortedSequencer* p_sortedSequencer, void* p_input, Callback* p_afterEveryExecutionCallback)
	{
		SortedSequencer_execute(p_sortedSequencer, p_input);
		Callback_call(p_afterEveryExecutionCallback, NULL);
	};

	SortedSequencerPriority SortedSequencer_calculatePriority(VectorT<SortedSequencerPriority>* p_before, VectorT<SortedSequencerPriority>* p_after)
	{


		if (!p_before && !p_after)
		{
			return 0;
		}

		if (p_before)
		{
			SortedSequencerPriority l_zeroPriority = (SortedSequencerPriority)0;
			
			if (CompareT_contains(VectorT_buildIterator(p_before), ComparatorT<SortedSequencerPriority, SortedSequencerPriority, void>{ SortedSequencerPriority_equals, &l_zeroPriority, nullptr }))
			{
				VectorT_free(p_before);
				if (p_after) { VectorT_free(p_after); }
				throw std::runtime_error("You cannot insert an operation before the priority 0 (which will always be the first).");
			}
		}

		bool l_returnValueFound = true;
		SortedSequencerPriority l_returnValue = 0;

		if (p_before && !p_after)
		{

			SortedSequencerPriority l_minBefore = USHRT_MAX;
			for (size_t i = 0; i < p_before->Size; i++)
			{
				SortedSequencerPriority l_priority = *VectorT_at(p_before, i);
				if (l_priority <= l_minBefore)
				{
					l_minBefore = l_priority;
				}
			}
			l_returnValue = l_minBefore - 1;
			goto end;
		}
		else if (!p_before && p_after)
		{
			SortedSequencerPriority l_maxAfter = 0;
			for (size_t i = 0; i < p_after->Size; i++)
			{
				SortedSequencerPriority l_priority = *VectorT_at(p_after, i);
				if (l_priority >= l_maxAfter)
				{
					l_maxAfter = l_priority;
				}
			}
			l_returnValue = l_maxAfter + 1;
			goto end;
		}
		else
		{
			SortedSequencerPriority l_minBefore = USHRT_MAX;
			for (size_t i = 0; i < p_before->Size; i++)
			{
				SortedSequencerPriority l_priority = *VectorT_at(p_before, i);
				if (l_priority <= l_minBefore)
				{
					l_minBefore = l_priority;
				}
			}

			SortedSequencerPriority l_maxAfter = 0;
			for (size_t i = 0; i < p_after->Size; i++)
			{
				SortedSequencerPriority l_priority = *VectorT_at(p_after, i);
				if (l_priority >= l_maxAfter)
				{
					l_maxAfter = l_priority;
				}
			}

			if (l_minBefore > l_maxAfter)
			{
				l_returnValue = l_minBefore - 1;
				goto end;
			}
			else if (l_minBefore == l_maxAfter)
			{
				l_returnValue = l_minBefore;
				goto end;
			}
			else
			{
				l_returnValueFound = false;
				goto end;
			}
		}

	end:

		{
			if (p_before) { VectorT_free(p_before); }
			if (p_after) { VectorT_free(p_after); }

			if (!l_returnValueFound)
			{
				throw std::runtime_error("Cannot find a suitable priority value that executes before and after the associated inputs.");
			}
		}
		return l_returnValue;


	};
}

/*
short int Core_SortedSequencerOperation_elementSorter(Core_SortedSequencerOperation* p_left, Core_SortedSequencerOperation* p_right, void* p_null)
{
	return Core_sortCompare_uint16_uint16(&p_left->Priority, &p_right->Priority);
}

void Core_SortedSequencer_alloc(Core_SortedSequencer* p_sequencer)
{
	Core_SortedVector_alloc(&p_sequencer->Operations, sizeof(Core_SortedSequencerOperation), 10, Core_SortedSequencerOperation_elementSorter);
};

void Core_SortedSequencer_free(Core_SortedSequencer* p_sequencer)
{
	Core_SortedVector_free(&p_sequencer->Operations);
};

void Core_SortedSequencer_addOperation(Core_SortedSequencer* p_sortedSequencer, Core_SortedSequencerOperation* p_sortedSequencerOperation)
{
	Core_SortedVector_pushBack(&p_sortedSequencer->Operations, p_sortedSequencerOperation);
};

void Core_SortedSequencer_execute(Core_SortedSequencer* p_sortedSequencer, void* p_input)
{
	Core_VectorIterator l_it = Core_GenericArray_buildIterator(&p_sortedSequencer->Operations.GenericArray);
	while (Core_VectorIterator_moveNext(&l_it))
	{
		Core_SortedSequencerOperation* l_oprtation = (Core_SortedSequencerOperation*)l_it.Current;
		Core_Callback_call(&l_oprtation->OperationCallback, p_input);
	}
};

void Core_SortedSequencer_execute_afterCallback(Core_SortedSequencer* p_sortedSequencer, void* p_input, Callback p_afterEveryExecutionCallback)
{
	Core_SortedSequencer_execute(p_sortedSequencer, p_input);
	Core_Callback_call(&p_afterEveryExecutionCallback, NULL);
};

SortedSequencerPriority Core_SortedSequencer_calculatePriority(CORE_VECTOR_NAME(SortedSequencerPriority)* p_before, CORE_VECTOR_NAME(SortedSequencerPriority)* p_after)
{

	if (!p_before && !p_after)
	{
		return 0;
	}

	if (p_before)
	{
		SortedSequencerPriority l_zeroPriority = (SortedSequencerPriority)0;

		Core_Comparator l_zeroPriority_comparator = { CORE_EQUALS(uint16, uint16), &l_zeroPriority, NULL };
		Core_VectorIterator l_beforeIterator = Core_GenericArray_buildIterator((Core_GenericArray*)p_before);

		if (Core_contains(&l_beforeIterator, &l_zeroPriority_comparator))
		{
			Core_GenericArray_free(p_before);
			if (p_after) { Core_GenericArray_free(p_after); }

			ERR_THROW_MESSAGE(CR_OUT_OF_BOUND, "You cannot insert an operation before the priority 0 (which will always be the first).")
		};
	}

	bool l_returnValueFound = true;
	SortedSequencerPriority l_returnValue = 0;

	if (p_before && !p_after)
	{

		SortedSequencerPriority l_minBefore = USHRT_MAX;
		for (size_t i = 0; i < p_before->Size; i++)
		{
			SortedSequencerPriority l_priority = *(SortedSequencerPriority*)Core_GenericArray_at(p_before, i);
			if (l_priority <= l_minBefore)
			{
				l_minBefore = l_priority;
			}
		}
		l_returnValue = l_minBefore - 1;
		goto end;
	}
	else if (!p_before && p_after)
	{
		SortedSequencerPriority l_maxAfter = 0;
		for (size_t i = 0; i < p_after->Size; i++)
		{
			SortedSequencerPriority l_priority = *(SortedSequencerPriority*)Core_GenericArray_at(p_after, i);
			if (l_priority >= l_maxAfter)
			{
				l_maxAfter = l_priority;
			}
		}
		l_returnValue = l_maxAfter + 1;
		goto end;
	}
	else
	{
		SortedSequencerPriority l_minBefore = USHRT_MAX;
		for (size_t i = 0; i < p_before->Size; i++)
		{
			SortedSequencerPriority l_priority = *(SortedSequencerPriority*)Core_GenericArray_at(p_before, i);
			if (l_priority <= l_minBefore)
			{
				l_minBefore = l_priority;
			}
		}

		SortedSequencerPriority l_maxAfter = 0;
		for (size_t i = 0; i < p_after->Size; i++)
		{
			SortedSequencerPriority l_priority = *(SortedSequencerPriority*)Core_GenericArray_at(p_after, i);
			if (l_priority >= l_maxAfter)
			{
				l_maxAfter = l_priority;
			}
		}

		if (l_minBefore > l_maxAfter)
		{
			l_returnValue = l_minBefore - 1;
			goto end;
		}
		else if (l_minBefore == l_maxAfter)
		{
			l_returnValue = l_minBefore;
			goto end;
		}
		else
		{
			l_returnValueFound = false;
			goto end;
		}
	}

end:

	{
		if (p_before) { Core_GenericArray_free(p_before); }
		if (p_after) { Core_GenericArray_free(p_after); }

		if (!l_returnValueFound)
		{
			ERR_THROW_MESSAGE(CR_OUT_OF_BOUND, "Cannot find a suitable priority value that executes before and after the associated inputs.");
		}
	}
	return l_returnValue;

};
*/