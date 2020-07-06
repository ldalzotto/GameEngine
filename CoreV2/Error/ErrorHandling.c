#pragma message (__FILE__)

CoreLib_ErrorHandler CoreLib_global_errorHandler;
CoreLib_ErrorHandlingObject* CoreLib_global_errorHandlingObject;

void CoreLib_ErrorHandling_pushToGlobal(CoreLib_ErrorHandlingObject* p_handlingObject)
{
	if (CoreLib_global_errorHandlingObject == NULL)
	{
		CoreLib_global_errorHandlingObject = malloc(sizeof(CoreLib_ErrorHandlingObject));
		memcpy(CoreLib_global_errorHandlingObject, p_handlingObject, sizeof(CoreLib_ErrorHandlingObject));
	}
	else
	{
		CoreLib_ErrorHandlingObject* l_child = CoreLib_global_errorHandlingObject;
		CoreLib_global_errorHandlingObject = malloc(sizeof(CoreLib_ErrorHandlingObject));
		memcpy(CoreLib_global_errorHandlingObject, p_handlingObject, sizeof(CoreLib_ErrorHandlingObject));
		CoreLib_global_errorHandlingObject->Child = l_child;
	}
};