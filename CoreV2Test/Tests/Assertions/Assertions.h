#pragma message(__FILE__)

#define CORETEST_ASSERT_EQUALS(left, right)	\
	CORE_HANDLE_ERROR_BEGIN(err) \
	if(left != right) \
	{ \
		err = CR_INVALID_INDICES; \
	} \
	CORE_HANDLE_ERROR_END(err);

#define CORETEST_ASSERT_SUPERIOR_STRICT(left, right) \
	CORE_HANDLE_ERROR_BEGIN(err) \
	if(left <= right) \
	{ \
		err = CR_INVALID_INDICES; \
	} \
	CORE_HANDLE_ERROR_END(err);

#define CORETEST_ASSERT_ERROR_BEGIN() \
	{ \
		CoreTest_CatchErrorObject l_catchErrorObject; \
		CoreTest_StartCatchingException(&l_catchErrorObject);

#define CORETEST_ASSERT_ERROR_END(in_awaitedReturnCode) \
		CoreTest_EndCatchingException(&l_catchErrorObject, in_awaitedReturnCode); \
	}
