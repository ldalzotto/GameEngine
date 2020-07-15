#include "SharedRAII.hpp"

#include <stdexcept>
#include "Log/LogFormatting.hpp"

namespace _Core
{

	void SharedRAII_use(SharedRAII* p_usageCounter)
	{
		p_usageCounter->UsageCount += 1;
	};
	
	void SharedRAII_release(SharedRAII* p_usageCounter)
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

		if (p_usageCounter->UsageCount == 0)
		{
			_Core::Callback_call(&p_usageCounter->ReleaseResource, p_usageCounter);
		}
	};
}