#pragma message ("########### CoreV2Consumer ###########")

#include "CoreV2Include.h"

#include "../Tests/MainTest.c"

int main()
{
	CoreLib_ErrorHandling_initialize();
	CoreTest_Execute();
	CoreLib_ErrorHandling_terminate();
}