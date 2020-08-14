#pragma once

#include "ResourceProvider.hpp"

namespace _Core
{
	template<typename RESOURCE_TYPE>
	struct Resource_HeaderT
	{
		size_t UsageCount;
		size_t Key;
		RESOURCE_TYPE* Memory;
	};

	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	using AllocateResourceT_Function = void(*)(Resource_HeaderT<RESOURCE_TYPE>* p_header, KEY_OBJECT* p_keyObject);
	template<typename RESOURCE_TYPE>
	using FreeResourceT_Function = void(*)(Resource_HeaderT<RESOURCE_TYPE>* p_header);

	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	struct ResourceAllocatorT
	{
		AllocateResourceT_Function<RESOURCE_TYPE, KEY_OBJECT> AllocFn;
		FreeResourceT_Function<RESOURCE_TYPE> FreeFn;
	};

	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	struct ResourceProviderT
	{
		VectorT<Resource_HeaderT<RESOURCE_TYPE>> Resources;
		size_t ResourceSize;
		ResourceAllocatorT<RESOURCE_TYPE, KEY_OBJECT> ResourceAllocator;
	};

	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	inline void ResourceProviderT_alloc(ResourceProviderT<RESOURCE_TYPE, KEY_OBJECT>* p_resourceProvider, ResourceAllocatorT<RESOURCE_TYPE, KEY_OBJECT>* p_resourceAllocator)
	{
		ResourceProvider_alloc((ResourceProvider*)p_resourceProvider, sizeof(RESOURCE_TYPE), (ResourceAllocator*)p_resourceAllocator);
	};
	
	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	inline void ResourceProviderT_free(ResourceProviderT<RESOURCE_TYPE, KEY_OBJECT>* p_resourceProvider)
	{
		ResourceProvider_free((ResourceProvider*)p_resourceProvider);
	};

	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	inline RESOURCE_TYPE* ResourceProviderT_useResource(ResourceProviderT<RESOURCE_TYPE, KEY_OBJECT>* p_resourceProvider, KEY_OBJECT* p_keyObject, size_t p_keyHashCode)
	{
		return (RESOURCE_TYPE*)ResourceProvider_useResource((ResourceProvider*)p_resourceProvider, p_keyObject, p_keyHashCode);
	};
	
	template<typename RESOURCE_TYPE, typename KEY_OBJECT>
	inline void ResourceProviderT_releaseResource(ResourceProviderT<RESOURCE_TYPE, KEY_OBJECT>* p_resourceProvider, size_t p_keyHashCode)
	{
		ResourceProvider_releaseResource((ResourceProvider*)p_resourceProvider, p_keyHashCode);
	};
}