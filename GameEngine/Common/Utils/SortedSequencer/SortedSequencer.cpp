#include "SortedSequencer.h"

#include "Utils/Algorithm/Algorithm.h"
#include <stdexcept>
#include "Log/Log.h"
#include <limits>

namespace _GameEngine::_Utils
{
	void SortedSequencer_addOperation(SortedSequencer* p_sortedSequencer, SortedSequencerOperation* p_sortedSequencerOperation)
	{
		size_t l_insertedIndex = 0;


		bool l_insertedIndexFound = false;
		for (size_t l_comparedIndex = 0; l_comparedIndex < p_sortedSequencer->Operations.size(); l_comparedIndex++)
		{
			SortedSequencerOperation* l_comparedOperation = &p_sortedSequencer->Operations.at(l_comparedIndex);
			if (p_sortedSequencerOperation->Priority <= l_comparedOperation->Priority)
			{
				l_insertedIndex = l_comparedIndex;
				l_insertedIndexFound = true;
				break;
			}
		}

		if (!l_insertedIndexFound)
		{
			l_insertedIndex = p_sortedSequencer->Operations.size();
		}

		p_sortedSequencer->Operations.insert(p_sortedSequencer->Operations.begin() + l_insertedIndex, *p_sortedSequencerOperation);

	};

	void SortedSequencer_execute(SortedSequencer* p_sortedSequencer, void* p_input)
	{
		for (SortedSequencerOperation& l_operation : p_sortedSequencer->Operations)
		{
			l_operation.Callback(l_operation.Closure, p_input);
		}
	};

	SortedSequencerPriority SortedSequencer_calculatePriority(std::vector<SortedSequencerPriority>* p_before, std::vector<SortedSequencerPriority>* p_after)
	{
		if (!p_before && !p_after)
		{
			return 0;
		}

		if (p_before)
		{
			if (Vector_containsElementEquals(p_before, (uint16_t)0) != -1 || Vector_containsElementEquals(p_before, (uint16_t)1) != -1)
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("You cannot insert an operation before the priority 0 (which will always be the first)."));
			};
		}

		if (p_before && !p_after)
		{

			uint16_t l_minBefore = std::numeric_limits<uint16_t>::max();
			for (size_t i = 0; i < p_before->size(); i++)
			{
				SortedSequencerPriority l_priority = p_before->at(i);
				if (l_priority <= l_minBefore)
				{
					l_minBefore = l_priority;
				}
			}
			return l_minBefore - 1;

		}
		else if (!p_before && p_after)
		{
			uint16_t l_maxAfter = 0;
			for (size_t i = 0; i < p_after->size(); i++)
			{
				SortedSequencerPriority l_priority = p_after->at(i);
				if (l_priority >= l_maxAfter)
				{
					l_maxAfter = l_priority;
				}
			}
			return l_maxAfter + 1;
		}
		else
		{
			uint16_t l_minBefore = std::numeric_limits<uint16_t>::max();
			for (size_t i = 0; i < p_before->size(); i++)
			{
				SortedSequencerPriority l_priority = p_before->at(i);
				if (l_priority <= l_minBefore)
				{
					l_minBefore = l_priority;
				}
			}

			uint16_t l_maxAfter = 0;
			for (size_t i = 0; i < p_after->size(); i++)
			{
				SortedSequencerPriority l_priority = p_after->at(i);
				if (l_priority >= l_maxAfter)
				{
					l_maxAfter = l_priority;
				}
			}

			if (l_minBefore > l_maxAfter)
			{
				return l_minBefore - 1;
			}
			else if (l_minBefore == l_maxAfter)
			{
				return l_minBefore;
			}
			else
			{
				throw std::runtime_error(LOG_BUILD_ERRORMESSAGE("Cannot find a suitable priority value that executes before and after the associated inputs."));
			}
		}
	};
}