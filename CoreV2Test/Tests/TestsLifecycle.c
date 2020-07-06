#pragma message(__FILE__)

void CoreTest_BeforeTest()
{
	CoreLib_ErrorHandler_initialize();
};

void CoreTest_AfterTest()
{
	CoreLib_ErrorHandler_terminate();
};

CoreLib_ErrorHandlerObject* CoreTest_Global_OriginalErrorHandlerObject;

CoreLib_ErrorHandlerObject* CoreTest_Global_TestErrorHandlerObject;

void CoreTest_CustomErrorHandler(CoreLib_ErrorHandlerObject* p_errorHandlerObject)
{
	((CoreTest_CatchErrorObject*)p_errorHandlerObject->UserObject)->ExceptionOccured = true;
	((CoreTest_CatchErrorObject*)p_errorHandlerObject->UserObject)->ReturnCode = CoreLib_global_errorHandlingObject->ReturnCode;
};

void CoreTest_StartCatchingException(CoreTest_CatchErrorObject* p_catchErrorObject)
{
	CoreTest_Global_OriginalErrorHandlerObject = CoreLib_global_errorHandler;
	p_catchErrorObject->ReturnCode = CR_OK;
	CoreTest_Global_TestErrorHandlerObject = calloc(1, sizeof(CoreLib_ErrorHandlerObject));
	CoreTest_Global_TestErrorHandlerObject->UserObject = p_catchErrorObject;
	CoreTest_Global_TestErrorHandlerObject->Handler = CoreTest_CustomErrorHandler;
	CoreLib_global_errorHandler = CoreTest_Global_TestErrorHandlerObject;
	p_catchErrorObject->ReturnCode = CR_OK;
};

void CoreTest_EndCatchingException(CoreTest_CatchErrorObject* p_catchErrorObject, Core_ReturnCodes p_awaitedErrorWode)
{
	CoreLib_global_errorHandler = CoreTest_Global_OriginalErrorHandlerObject;
	CORE_HANDLE_ERROR_BEGIN(err);
	if (!p_catchErrorObject->ExceptionOccured || p_catchErrorObject->ReturnCode != p_awaitedErrorWode)
	{
		err = CR_OUT_OF_BOUND;
	}
	CORE_HANDLE_ERROR_END(err);
};