#pragma once

namespace _Core
{
	template<class VALUE_TYPE>
	struct OptionalT
	{
		bool HasValue;
		VALUE_TYPE Value;
	};

	template <class VALUE_TYPE>
	VALUE_TYPE* OptionalT_getValue(OptionalT<VALUE_TYPE>* p_optionalValue)
	{
		if (p_optionalValue->HasValue) { return &p_optionalValue->Value; }
		else { return nullptr; }
	};
}