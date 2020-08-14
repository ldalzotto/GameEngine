#pragma once

#include "DataStructures/Specifications/VectorT.hpp"
#include "Functional/Callback/Callback.hpp"

namespace _Core
{
	struct Resource_Header
	{
		size_t UsageCount;
		size_t Key;
		void* Memory;
	};

	using KeyObject_PTR = void*;

	typedef void(*AllocateResource_Function)(void* p_resource, KeyObject_PTR p_keyObject);
	typedef void(*FreeResource_Function)(void* p_resource);

	struct ResourceAllocator
	{
		AllocateResource_Function AllocFn;
		FreeResource_Function FreeFn;
	};

	struct ResourceProvider
	{
		VectorT<Resource_Header> Resources;
		size_t ResourceSize;
		ResourceAllocator ResourceAllocator;
	};

	void ResourceProvider_alloc(ResourceProvider* p_resourceProvider, size_t p_resourceSize, ResourceAllocator* p_resourceAllocator);
	void ResourceProvider_free(ResourceProvider* p_resourceProvider);

	void* ResourceProvider_useResource(ResourceProvider* p_resourceProvider, KeyObject_PTR p_keyObject, size_t p_keyHashCode);
	void ResourceProvider_releaseResource(ResourceProvider* p_resourceProvider, size_t p_keyHashCode);
}