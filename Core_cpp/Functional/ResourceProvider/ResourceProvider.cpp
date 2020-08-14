#include "ResourceProvider.hpp"
#include "Functional/Comparator/ComparatorT.hpp"
#include <stdlib.h>

namespace _Core
{
	bool resourceProvider_compare_function(Resource_Header* p_resource, size_t* p_key, void*)
	{
		return p_resource->Key == *p_key;
	};

	void ResourceProvider_alloc(ResourceProvider* p_resourceProvider, size_t p_resourceSize, ResourceAllocator* p_resourceAllocator)
	{
		p_resourceProvider->ResourceAllocator = *p_resourceAllocator;
		p_resourceProvider->ResourceSize = p_resourceSize;
		_Core::VectorT_alloc(&p_resourceProvider->Resources, 0);
	};

	void ResourceProvider_free(ResourceProvider* p_resourceProvider)
	{
		_Core::VectorT_free(&p_resourceProvider->Resources);
	};

	void* ResourceProvider_useResource(ResourceProvider* p_resourceProvider, KeyObject_PTR p_keyObject, size_t p_keyHashCode)
	{
		ComparatorT<Resource_Header, size_t, void> l_comparator{};
		l_comparator.ComparedObject = &p_keyHashCode;
		l_comparator.Function = resourceProvider_compare_function;

		VectorIteratorT<Resource_Header> l_resourceHeaderFound;
		if (VectorT_find(&p_resourceProvider->Resources, &l_comparator, &l_resourceHeaderFound))
		{
			l_resourceHeaderFound.Current->UsageCount += 1;
			return l_resourceHeaderFound.Current;
		}
		else
		{
			Resource_Header l_inStanciatedResource{};
			l_inStanciatedResource.UsageCount = 1;
			l_inStanciatedResource.Key = p_keyHashCode;
			l_inStanciatedResource.Memory = calloc(1, p_resourceProvider->ResourceSize);
			p_resourceProvider->ResourceAllocator.AllocFn(l_inStanciatedResource.Memory, p_keyObject);
			Resource_Header* l_header = VectorT_pushBack(&p_resourceProvider->Resources, &l_inStanciatedResource);
			return l_inStanciatedResource.Memory;
		}
	};

	void ResourceProvider_releaseResource(ResourceProvider* p_resourceProvider, size_t p_keyHashCode)
	{
		ComparatorT<Resource_Header, size_t, void> l_comparator{};
		l_comparator.ComparedObject = &p_keyHashCode;
		l_comparator.Function = resourceProvider_compare_function;

		VectorIteratorT<Resource_Header> l_resourceHeaderFound;
		if (VectorT_find(&p_resourceProvider->Resources, &l_comparator, &l_resourceHeaderFound))
		{
			l_resourceHeaderFound.Current->UsageCount -= 1;
			if (l_resourceHeaderFound.Current->UsageCount == 0)
			{
				p_resourceProvider->ResourceAllocator.FreeFn(l_resourceHeaderFound.Current->Memory);
				free(l_resourceHeaderFound.Current->Memory);
				_Core::VectorT_erase(&p_resourceProvider->Resources, l_resourceHeaderFound.CurrentIndex);
			}
		}
	};


}