#pragma once

#include <cstdint>

namespace _GameEngine::_Utils
{
	struct UsageCounter
	{
		uint16_t UsageCount;
	};

	void UsageCounter_use(UsageCounter* p_usageCounter);
	void UsageCounter_release(UsageCounter* p_usageCounter);
}