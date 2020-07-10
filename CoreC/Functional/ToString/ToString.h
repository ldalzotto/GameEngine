#pragma once

void Core_toString_float(struct Core_GenericArray* p_string, float* p_element);

void Core_toString_int(struct Core_GenericArray* p_string, int* p_element);
void Core_toString_intv(char p_string[50], int* p_element);
#define TOSTRING_INT(IntVar, IntPtr) char IntVar[50]; Core_toString_intv(IntVar, IntPtr);