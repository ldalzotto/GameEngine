#pragma once

namespace _GameEngine::_Core
{
	struct String;
}

namespace _GameEngine::_Core
{
	void String_append(_Core::String* p_string, char* p_appended, size_t p_charNb);
	void String_append(_Core::String* p_string, char* p_appended);
	bool String_find(_Core::String* p_string, char* p_comparedStr, size_t p_comparedCharNb, size_t p_startIndex, size_t* p_outfoundIndex);
	void String_substring(_Core::String* p_string, size_t p_beginIncluded, size_t p_endIncluded, _Core::String* p_newString);
	bool String_equals(_Core::String* p_string, char* p_comparedStr);

#define STRING_FIND(StringPtr, ComparedStringPtr, StartIndex, OutFoundIndexPtr) _GameEngine::_Core::String_find(##StringPtr, (##ComparedStringPtr)->c_str(), String_charNb(##ComparedStringPtr), ##StartIndex, ##OutFoundIndexPtr)
}