#include "Errorhandling.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

CoreLib_ErrorHandlerObject* CoreLib_global_errorHandler;
CoreLib_ErrorHandlingObject* CoreLib_global_errorHandlingObject;

void CoreLib_DefaultErrorHandle(struct CoreLib_ErrorHandlerObject*);
void CoreLib_ErrorHandlerObject_free(CoreLib_ErrorHandlerObject** p_errorHandlerObject);

void CoreLib_ErrorHandler_initialize()
{
    CoreLib_global_errorHandler = calloc(1, sizeof(CoreLib_ErrorHandlerObject));
    CoreLib_global_errorHandler->Handler = CoreLib_DefaultErrorHandle;
};

void CoreLib_ErrorHandler_terminate()
{
    CoreLib_ErrorHandlerObject_free(&CoreLib_global_errorHandler);
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
    if (CoreLib_global_errorHandler != NULL)
    {
        if (CoreLib_global_errorHandler->Handler != NULL) { CoreLib_global_errorHandler->Handler(CoreLib_global_errorHandler); }

    }
    CoreLib_ErrorHandler_terminate();
};

void CoreLib_DefaultErrorHandle(struct CoreLib_ErrorHandlerObject* p_errorhandlerObject)
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
        puts(l_buffer);
        l_currentObject = l_currentObject->Child;
    }

    exit(EXIT_FAILURE);
};