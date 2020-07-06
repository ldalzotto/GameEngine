#pragma message(__FILE__)

void CoreTest_BeforeTest();
void CoreTest_AfterTest();

typedef struct CoreTest_CatchErrorObject
{
	bool ExceptionOccured;
	Core_ReturnCodes ReturnCode;
} CoreTest_CatchErrorObject;

void CoreTest_StartCatchingException(CoreTest_CatchErrorObject* p_catchErrorObject);
void CoreTest_EndCatchingException(CoreTest_CatchErrorObject* p_catchErrorObject);