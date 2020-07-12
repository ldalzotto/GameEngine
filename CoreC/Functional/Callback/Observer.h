#pragma once

#include "Functional/Callback/Callback.h"
#include "DataStructures/GenericArray.h"
#include "DataStructures/GenericArrayNameMacros.h"

typedef void(*SujectCallback)(void* p_closure, void* p_observerInput);

typedef struct Core_Observer
{
	CORE_VECTOR_NAME(Callback) Subjects;
} Core_Observer;

void Core_ObserverAlloc(Core_Observer* p_observer);
void Core_ObserverFree(Core_Observer* p_observer);
void Core_Observer_register(Core_Observer* p_observer, Callback* p_subject);
void Core_Observer_unRegister(Core_Observer* p_observer, Callback* p_subject);
void Core_Observer_broadcast(Core_Observer* p_observer, void* p_input);