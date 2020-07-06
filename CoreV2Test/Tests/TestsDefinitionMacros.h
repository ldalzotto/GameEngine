#pragma message(__FILE__)

#define CORETEST_BEGIN(TestName) \
void TestName() { \
	CoreTest_BeforeTest();

#define CORETEST_END() \
	CoreTest_AfterTest(); \
}
