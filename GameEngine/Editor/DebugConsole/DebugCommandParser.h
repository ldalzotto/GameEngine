#pragma once

#include <string>
#include <vector>

namespace _GameEngineEditor
{
	struct DebugCommandArgument
	{
		std::string Name;
		std::string Value;
	};

	struct DebugCommand
	{
		std::string Verb;
		std::vector<DebugCommandArgument> Arguments;
	};

	enum DebugCommandErrorCode : uint8_t
	{
		OK = 0,
		VERB_NOT_FOUND = 1,
		ARGUMENT_PARSING = 2
	};

	DebugCommandErrorCode DebugCommand_parse(DebugCommand* p_debugCommand, std::string* p_input);
	void DebugCommand_handleError(DebugCommandErrorCode p_debugCommandErrorCode, std::string* out_errorMessage);
}