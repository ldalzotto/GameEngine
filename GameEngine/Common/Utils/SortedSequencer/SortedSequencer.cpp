#include "SortedSequencer.h"

#include "Utils/Algorithm/Algorithm.h"
#include "Utils/Functional/Comparators.h"
#include <stdexcept>
#include "MYLog/MYLog.h"
#include <limits>

namespace _GameEngine::_Utils
{
	bool SortedSequencerPriority_equals(SortedSequencerPriority* p_left, SortedSequencerPriority* p_right)
	{
		return *p_left == *p_right;
	};

	short SortedSequencerOperation_sortComparator(SortedSequencerOperation* p_left, SortedSequencerOperation* p_right)
	{
		return Uint16tComparator(p_left->Priority, p_right->Priority);
	}

	void SortedSequencer_alloc(SortedSequencer* p_sortedSequencer)
	{
		p_sortedSequencer->OperationsV2.alloc(2, SortedSequencerOperation_sortComparator);
	};

	void SortedSequender_free(SortedSequencer* p_sortedSequencer)
	{
		p_sortedSequencer->OperationsV2.free();
	};

	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation)
	{
		p_sortedSequencer->OperationsV2.push_back(p_sortedSequencerOperation);

	};

	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input)
	{
		for (size_t i = 0; i < p_sortedSequencer->OperationsV2.vector()->size(); i++)
		{
			SortedSequencerOperation* l_operation = p_sortedSequencer->OperationsV2.vector()->at(i);
			l_operation->Callback(l_operation->Closure, p_input);
		}
	};

	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input, void(*p_afterEveryExecution)(void*), void* p_afterEveryExecution_userData)
	{
		for (size_t i = 0; i < p_sortedSequencer->OperationsV2.vector()->size(); i++)
		{
			SortedSequencerOperation* l_operation = p_sortedSequencer->OperationsV2.vector()->at(i);
			l_operation->Callback(l_operation->Closure, p_input);
			p_afterEveryExecution(p_afterEveryExecution_userData);
		}
	};

	SortedSequencerPriority SortedSequencer_calculatePriority(_Core::VectorT<SortedSequencerPriority>* p_before, _Core::VectorT<SortedSequencerPriority>* p_after)
	{
		if (!p_before && !p_after)
		{
			return 0;
		}

		if (p_before)
		{
			SortedSequencerPriority l_zeroPriority = (uint16_t)0;
			SortedSequencerPriority l_onePriority = (uint16_t)1;

			if (p_before->contains(SortedSequencerPriority_equals, &l_zeroPriority) || p_before->contains(SortedSequencerPriority_equals, &l_onePriority))
			{
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("You cannot insert an operation before the priority 0 (which will always be the first)."));
			};
		}

		bool l_returnValueFound = true;
		SortedSequencerPriority l_returnValue = 0;

		if (p_before && !p_after)
		{

			uint16_t l_minBefore = std::numeric_limits<uint16_t>::max();
			for (size_t i = 0; i < p_before->size(); i++)
			{
				SortedSequencerPriority l_priority = *p_before->at(i);
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
			uint16_t l_maxAfter = 0;
			for (size_t i = 0; i < p_after->size(); i++)
			{
				SortedSequencerPriority l_priority = *p_after->at(i);
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
			uint16_t l_minBefore = std::numeric_limits<uint16_t>::max();
			for (size_t i = 0; i < p_before->size(); i++)
			{
				SortedSequencerPriority l_priority = *p_before->at(i);
				if (l_priority <= l_minBefore)
				{
					l_minBefore = l_priority;
				}
			}

			uint16_t l_maxAfter = 0;
			for (size_t i = 0; i < p_after->size(); i++)
			{
				SortedSequencerPriority l_priority = *p_after->at(i);
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
			if (p_before)
			{
				p_before->free();
			}
			if (p_after)
			{
				p_after->free();
			}

			if (!l_returnValueFound)
			{
				throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("Cannot find a suitable priority value that executes before and after the associated inputs."));
			}
		}
		
		return l_returnValue;
	};
}