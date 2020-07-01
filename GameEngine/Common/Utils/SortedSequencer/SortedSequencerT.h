#pragma once

#define SORTED_SEQUENCER_CALCULATEPRIORITY_ALLOCATE_BEFORE(TotalNumber); \
	_Core::VectorT<_Utils::SortedSequencerPriority> l_before; \
	l_before.alloc(##TotalNumber);

#define SORTED_SEQUENCER_CALCULATEPRIORITY_SET_BEFORE(BeforeProviderFnName) \
	{ \
		auto l_index = ##BeforeProviderFnName(); \
		l_before.push_back(&l_index); \
	}

#define SORTED_SEQUENCER_CALCULATEPRIORITY_CALCULATE_B(Out_priorityValueName) \
	_Utils::SortedSequencerPriority Out_priorityValueName = _Utils::SortedSequencer_calculatePriority(&l_before, nullptr); \
	l_before.free(); \
	return Out_priorityValueName;