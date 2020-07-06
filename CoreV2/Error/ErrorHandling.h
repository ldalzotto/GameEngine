#pragma message (__FILE__)

typedef struct CoreLib_ErrorHandlingObject
{
	struct CoreLib_ErrorHandlingObject* Child;
	Core_ReturnCodes ReturnCode;
	char* FileName;
	int LineNb;
	char* CustomMessage;
} CoreLib_ErrorHandlingObject;

typedef void(*CoreLib_ErrorHandler)();

extern CoreLib_ErrorHandler CoreLib_global_errorHandler;
extern CoreLib_ErrorHandlingObject* CoreLib_global_errorHandlingObject;

void CoreLib_ErrorHandling_pushToGlobal(CoreLib_ErrorHandlingObject* p_handlingObject);

#define CORE_HANDLE_PUSH_GLOBAL(in_returnCode, in_message) \
	if(##in_returnCode != CR_OK) { \
		CoreLib_ErrorHandlingObject l_errorHandling = {NULL, ##in_returnCode, __FILE__, __LINE__, ##in_message}; \
		CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
		return in_returnCode; \
	}

#define CORE_HANDLE_PUSH_GLOBAL_BEGIN(var_returnCode) \
	{ \
	Core_ReturnCodes var_returnCode = CR_OK;;

#define CORE_HANDLE_PUSH_GLOBAL_END(in_returnCode, in_message) \
	CORE_HANDLE_PUSH_GLOBAL(in_returnCode, in_message); \
	}

#define CORE_HANDLE_ERROR_BEGIN(var_returnCode) \
	{ \
	Core_ReturnCodes var_returnCode = CR_OK;

#define CORE_HANDLE_ERROR_END(in_returnCode) \
		if (##in_returnCode != CR_OK) \
		{ \
			CoreLib_ErrorHandlingObject l_errorHandling = {NULL, ##in_returnCode, __FILE__, __LINE__, "Error top level."}; \
			CoreLib_ErrorHandling_pushToGlobal(&l_errorHandling); \
			CoreLib_global_errorHandler(); \
		} \
	}
