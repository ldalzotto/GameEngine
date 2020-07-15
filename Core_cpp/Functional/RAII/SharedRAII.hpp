#pragma once

#include <stdint.h>
#include "Functional/Callback/Callback.hpp"

namespace _Core
{
	struct SharedRAII
	{
		uint16_t UsageCount;
		void* Resource;
		Callback ReleaseResource;
	};
	void SharedRAII_use(SharedRAII* p_usageCounter);
	void SharedRAII_release(SharedRAII* p_usageCounter);
}