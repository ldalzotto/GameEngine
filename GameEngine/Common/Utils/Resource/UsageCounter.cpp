#include "UsageCounter.h"

#include <stdexcept>
#include "MyLog/MyLog.h"

namespace _GameEngine::_Utils
{
	void UsageCounter_use(UsageCounter* p_usageCounter)
	{
		p_usageCounter->UsageCount += 1;
	};

	void UsageCounter_release(UsageCounter* p_usageCounter)
	{
		if (p_usageCounter->UsageCount > 0)
		{
			p_usageCounter->UsageCount -= 1;
		}
#ifndef NDEBUG
		else
		{

			throw std::runtime_error(MYLOG_BUILD_ERRORMESSAGE("The UsageCounter cannot be released whe it's current UsageCount is 0!"));
		}
#endif
	};
}