#pragma once
#include <stdio.h>
#include <stdlib.h>

extern char RETURN_CODE;

#ifndef NEDBUG
#define HANDLE_ERR(FuncCall) \
{RETURN_CODE = FuncCall; if(RETURN_CODE) { printf("Critical error in file:%s, function:%s() and line:%d", __FILE__, __func__, __LINE__);  abort(); } }
#else
#define HANDLE_ERR(FuncCall) \
FuncCall
#endif // !NEDBUG

#define HANDLE_ERR_SILENT(FuncCall) \
{RETURN_CODE = FuncCall; if(RETURN_CODE) {return RETURN_CODE;} }