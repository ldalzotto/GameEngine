#pragma once

#include "ReturnCodes.h"

typedef struct CoreLib_ErrorHandlingObject
{
    struct CoreLib_ErrorHandlingObject* Child;
    Core_ReturnCodes ReturnCode;
    char* FileName;
    int LineNb;
    char* CustomMessage;
} CoreLib_ErrorHandlingObject;
typedef CoreLib_ErrorHandlingObject* CoreLib_ErrorHandlingObject_Handle;

extern CoreLib_ErrorHandlingObject* CoreLib_global_errorHandlingObject;

void CoreLib_ErrorHandling_pushToGlobal(CoreLib_ErrorHandlingObject* p_handlingObject);
void CoreLib_ErrorHandling_handleError();

struct CoreLib_ErrorHandlerObject;

typedef void(*CoreLib_ErrorHandlerFunction)(struct CoreLib_ErrorHandlerObject* p_errorHandlerObject);
typedef struct CoreLib_ErrorHandlerObject
{
    void* UserObject;
    CoreLib_ErrorHandlerFunction Handler;
} CoreLib_ErrorHandlerObject;

extern CoreLib_ErrorHandlerObject* CoreLib_global_errorHandler;

void CoreLib_ErrorHandler_initialize();
void CoreLib_ErrorHandler_terminate();

#define ERR_PASS(Expression, Message) \
	{ \
		Core_ReturnCodes var_returnCode = Expression; \
		if (var_returnCode != CR_OK) \
		{ \
				CoreLib_ErrorHandlingObject l_errorHandling = {NULL, var_returnCode, __FILE__, __LINE__, Message}; \
				CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
				return var_returnCode; \
		} \
	}

#define ERR_PUSH(ReturnCode, Message) \
	{ \
		if (ReturnCode != CR_OK) \
		{ \
				CoreLib_ErrorHandlingObject l_errorHandling = {NULL, ReturnCode, __FILE__, __LINE__, Message}; \
				CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
				return ReturnCode; \
		} \
	}

#define ERR_THROW(Expression) \
	{ \
		Core_ReturnCodes var_returnCode = Expression; \
		if (var_returnCode != CR_OK) \
		{ \
				CoreLib_ErrorHandlingObject l_errorHandling = {NULL, var_returnCode, __FILE__, __LINE__, "Error top level."}; \
				CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
				CoreLib_ErrorHandling_handleError(); \
		} \
	}

#define ERR_THROW_MESSAGE(Expression, Message) \
	{ \
		Core_ReturnCodes var_returnCode = Expression; \
		if (var_returnCode != CR_OK) \
		{ \
				CoreLib_ErrorHandlingObject l_errorHandling = {NULL, var_returnCode, __FILE__, __LINE__, Message}; \
				CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
				CoreLib_ErrorHandling_handleError(); \
		} \
	}
