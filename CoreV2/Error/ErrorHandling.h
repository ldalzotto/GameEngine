#pragma message (__FILE__)

typedef struct CoreLib_ErrorHandlingObject
{
	struct CoreLib_ErrorHandlingObject* Child;
	Core_ReturnCodes ReturnCode;
	char* FileName;
	int LineNb;
	char* CustomMessage;
} CoreLib_ErrorHandlingObject;
typedef CoreLib_ErrorHandlingObject* CoreLib_ErrorHandlingObject_Handle;

typedef void(*CoreLib_ErrorHandler)();

typedef struct CoreLib_ErrorHandlerObject
{
	struct CoreLib_ErrorHandlerObject* HandlerChain;
	CoreLib_ErrorHandler Handler;
} CoreLib_ErrorHandlerObject;

void CoreLib_ErrorHandling_initialize();
void CoreLib_ErrorHandling_terminate();

void CoreLib_ErrorHandling_pushToGlobal(CoreLib_ErrorHandlingObject* p_handlingObject);
void COreLib_ErrorHandling_handleError();

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
			COreLib_ErrorHandling_handleError(); \
		} \
	}
