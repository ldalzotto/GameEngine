#pragma message(__FILE__)

void CoreTest_BeforeTest()
{
	CoreLib_ErrorHandler_initialize();
};

void CoreTest_AfterTest()
{
	CoreLib_ErrorHandler_terminate();
};