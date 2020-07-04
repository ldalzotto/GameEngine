
#include "ArrayTest.h"
extern "C" {
#include "Error/ErrorHandling.h"
}
#include <stdexcept>

void Core_HandleError()
{
	printf("--------------- ERROR ---------------\n");
	
	CoreLib_ErrorHandlingObject* l_currentObject = CoreLib_global_errorHandlingObject;
	char l_buffer[2000];
	
	while (l_currentObject != NULL)
	{
		memset(l_buffer, 0, sizeof(l_buffer));
		strcat(l_buffer, l_currentObject->FileName);
		strcat(l_buffer, "_");
		char l_lineNbChr[10];
		sprintf(l_lineNbChr, "%i", l_currentObject->LineNb);
		strcat(l_buffer, l_lineNbChr);
		strcat(l_buffer, " : ");
		strcat(l_buffer, l_currentObject->CustomMessage);
		strcat(l_buffer, "\n");
		printf(l_buffer);
		l_currentObject = l_currentObject->Child;
	}

	exit(EXIT_FAILURE);
}

int main()
{
	CoreLib_global_errorHandler = Core_HandleError;
	_Core::_Test::ExecuteTest();
}