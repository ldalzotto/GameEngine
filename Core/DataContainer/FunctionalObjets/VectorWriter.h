#pragma once

typedef struct Core_VectorWriter
{
	void* Core_VectorWriter_UserObject;
	void (*PushBack)(void* p_userObject, void* p_value);
} Core_VectorWriter;
