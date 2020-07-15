#pragma once

#include "SharedRAII.hpp"
#include "Functional/Callback/CallbackT.hpp"

namespace _Core
{
	template <typename RESOURCE_TYPE, typename RELEASE_CALLBACK_CLOSURE_TYPE>
	struct SharedRAIIT
	{
		uint16_t UsageCount;
		RESOURCE_TYPE* Resource;
		CallbackT<RELEASE_CALLBACK_CLOSURE_TYPE, SharedRAIIT<RESOURCE_TYPE, RELEASE_CALLBACK_CLOSURE_TYPE>> ReleaseResource;
	};
	
	template <typename RESOURCE_TYPE, typename RELEASE_CALLBACK_CLOSURE_TYPE>
	inline void SharedRAIIT_use(SharedRAIIT<RESOURCE_TYPE, RELEASE_CALLBACK_CLOSURE_TYPE>* p_usageCounter)
	{
		SharedRAII_use((SharedRAII*)p_usageCounter);
	};
	
	template <typename RESOURCE_TYPE, typename RELEASE_CALLBACK_CLOSURE_TYPE>
	inline void SharedRAIIT_release(SharedRAIIT<RESOURCE_TYPE, RELEASE_CALLBACK_CLOSURE_TYPE>* p_usageCounter)
	{
		SharedRAII_release((SharedRAII*)p_usageCounter);
	};
}