#pragma message (__FILE__)

CoreLib_ErrorHandlerObject* CoreLib_global_errorHandler;
CoreLib_ErrorHandlingObject* CoreLib_global_errorHandlingObject;

typedef CoreLib_ErrorHandlerObject* CoreLib_ErrorHandlerObjectHandle;
CORE_VECTORACCESSOR_DEFINE(Core_CoreLib_ErrorHandlerObjectHandle_Vector, CoreLib_ErrorHandlerObjectHandle);
CORE_VECTORWRITER_DEFINE(Core_CoreLib_ErrorHandlerObjectHandle_Vector, CoreLib_ErrorHandlerObjectHandle);
CORE_DEFINE_VECTOR(CoreLib_ErrorHandlerObjectHandle);
CORE_VECTORITERATOR_DEFINE(Core_CoreLib_ErrorHandlerObjectHandle_Vector, CoreLib_ErrorHandlerObjectHandle);
CORE_DEFINE_VECTOR_IMPL(CoreLib_ErrorHandlerObjectHandle);

void CoreLib_DefaultErrorHandle();
void CoreLib_ErrorHandlerObject_free(CoreLib_ErrorHandlerObject** p_errorHandlerObject);

void CoreLib_ErrorHandler_initialize()
{
	CoreLib_global_errorHandler = calloc(1, sizeof(CoreLib_ErrorHandlerObject));
	CoreLib_global_errorHandler->Handler = CoreLib_DefaultErrorHandle;
};

void CoreLib_ErrorHandling_chainErrorHandler(CoreLib_ErrorHandlerFunction p_handler)
{
	CoreLib_ErrorHandlerObject* l_newParentObject = calloc(1, sizeof(CoreLib_ErrorHandlerObject));
	l_newParentObject->Handler = p_handler;

	if (CoreLib_global_errorHandler != NULL)
	{
		l_newParentObject->HandlerChain = CoreLib_global_errorHandler;
	}

	CoreLib_global_errorHandler = l_newParentObject;
};

void CoreLib_ErrorHandler_terminate()
{
	if (CoreLib_global_errorHandler != NULL)
	{
		CORE_VECTOR_ALLOC(CoreLib_ErrorHandlerObjectHandle, 4, l_stackedErrorHandlers);
		{
			{
				CoreLib_ErrorHandlerObject* l_currentErrorHandlerObject = CoreLib_global_errorHandler;
				while (l_currentErrorHandlerObject != NULL)
				{
					CORE_VECTORWRITER_PUSHBACK(&l_stackedErrorHandlers, &l_currentErrorHandlerObject);
					l_currentErrorHandlerObject = l_currentErrorHandlerObject->HandlerChain;
				}
			}

			CORE_VECTORITERATOR_FOREACH_BEGIN(Core_CoreLib_ErrorHandlerObjectHandle_Vector, &l_stackedErrorHandlers, l_it)
				CoreLib_ErrorHandlerObject_free(l_it.Current);
			CORE_VECTORITERATOR_FOREACH_END();

			CoreLib_global_errorHandler = NULL;
		}
		CORE_VECTOR_FREE(CoreLib_ErrorHandlerObjectHandle, &l_stackedErrorHandlers)
	}
};

void CoreLib_ErrorHandlerObject_free(CoreLib_ErrorHandlerObject** p_errorHandlerObject)
{
	free((*p_errorHandlerObject));
	(*p_errorHandlerObject) = NULL;
};

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

void CoreLib_ErrorHandling_handleError()
{
	CoreLib_ErrorHandlerObject* l_currentErrorHandler = CoreLib_global_errorHandler;
	while (l_currentErrorHandler != NULL)
	{
		if (l_currentErrorHandler->Handler != NULL) { l_currentErrorHandler->Handler(); }
		l_currentErrorHandler = l_currentErrorHandler->HandlerChain;
	}
	CoreLib_ErrorHandler_terminate();
};

void CoreLib_DefaultErrorHandle()
{
	printf("--------------- ERROR ---------------\n");

	CoreLib_ErrorHandlingObject* l_currentObject = CoreLib_global_errorHandlingObject;
	char l_buffer[2000];
	Core_char_Array l_bufferString;
	Core_char_Array_fromRawArray(&l_bufferString, &l_buffer, 2000);

	while (l_currentObject != NULL)
	{
		CORE_STRING_CLEAR(Core_char_Array, &l_bufferString);

		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, l_currentObject->FileName);
		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, "_");
		char l_lineNbChr[10];
		sprintf(l_lineNbChr, "%i", l_currentObject->LineNb);
		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, l_lineNbChr);
		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, " : ");
		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, l_currentObject->CustomMessage);
		CORE_STRING_APPEND(Core_char_Array, &l_bufferString, "\n");
		printf(l_buffer);
		l_currentObject = l_currentObject->Child;
	}

	exit(EXIT_FAILURE);
};