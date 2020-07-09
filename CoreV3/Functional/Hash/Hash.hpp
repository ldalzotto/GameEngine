#pragma message(__FILE__)

namespace _CoreV3
{

	template <typename T>
	inline size_t Hash(T* p_value)
	{
		return std::hash<T>()(*p_value);
	};

	template < typename T, template < class T > class DATA_STRUCTURE >
	inline size_t Hash(DATA_STRUCTURE<T>* p_value)
	{
		return Hash(p_value->Memory);
	}

	template <typename HASHED_TYPE>
	inline size_t HashCombine(size_t p_seed, HASHED_TYPE* p_right)
	{
		return p_seed ^ (Hash(p_right) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
	}

	template <typename HASHED_TYPE>
	inline size_t HashCombine(size_t p_seed, HASHED_TYPE&& p_right)
	{
		return p_seed ^ (Hash(&p_right) + 0x9e3779b9 + (p_seed << 6) + (p_seed >> 2));
	}

	template <typename HASHED_TYPE, typename... HASHED_TYPES>
	inline size_t HashCombine(size_t p_seed, HASHED_TYPE* p_hashedType, HASHED_TYPES... p_right)
	{
		return HashCombine(HashCombine(p_seed, p_hashedType), p_right...);
	}
	
}