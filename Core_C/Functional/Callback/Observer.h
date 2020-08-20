#pragma once

#include "Observer_def.h"

void Observer_Alloc(Observer_PTR p_observer);
void Observer_Free(Observer_PTR p_observer);
void Observer_Register(Observer_PTR p_observer, Callback_PTR p_subject);
void Observer_UnRegister(Observer_PTR p_observer, Callback_PTR p_subject);
void Observer_Broadcast(Observer_PTR p_observer, void* p_input);
