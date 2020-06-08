#include "DebugCommandParser.h"


namespace _GameEngineEditor
{
	DebugCommandErrorCode DebugCommand_parse(DebugCommand* p_debugCommand, std::string* p_input)
	{
		std::string l_inputCopy = *p_input;

		{
			size_t l_verbFoundPos = l_inputCopy.find(' ');
			if (l_verbFoundPos != std::string::npos)
			{
				p_debugCommand->Verb = l_inputCopy.substr(0, l_verbFoundPos);
				l_inputCopy.erase(0, l_verbFoundPos + 1);
			}
			else
			{
				return DebugCommandErrorCode::VERB_NOT_FOUND;
			}
		}

		{
			while (l_inputCopy.size() > 0)
			{
				bool l_argumentProcessedSuccessfully = false;
				size_t l_argumentFrontFoundPos = l_inputCopy.find('-');
				if (l_argumentFrontFoundPos != std::string::npos)
				{
					size_t l_argumentEndPos;
					l_argumentEndPos = l_inputCopy.find("\" -", l_argumentFrontFoundPos);
					if (l_argumentEndPos == std::string::npos)
					{
						l_argumentEndPos = l_inputCopy.find_last_of("\"\0");
						if (l_argumentEndPos == std::string::npos)
						{
							return DebugCommandErrorCode::ARGUMENT_PARSING;
						}
					}

					if (l_argumentEndPos != std::string::npos)
					{
						std::string l_argumentFullString = l_inputCopy.substr(l_argumentFrontFoundPos, l_argumentEndPos + 1);


						size_t l_argumentNameEndFoundPos = l_argumentFullString.find('=', l_argumentFrontFoundPos);
						if (l_argumentNameEndFoundPos != std::string::npos)
						{
							std::string l_argumentName = l_argumentFullString.substr(1, l_argumentNameEndFoundPos - 1);
							size_t l_last = l_argumentFullString.find_last_of("\"");
							std::string l_argumentValue = l_argumentFullString.substr(l_argumentNameEndFoundPos + 2, l_argumentFullString.find_last_of("\"") - (l_argumentNameEndFoundPos + 2));

							DebugCommandArgument l_arg{};
							l_arg.Name = l_argumentName;
							l_arg.Value = l_argumentValue;
							p_debugCommand->Arguments.push_back(l_arg);
							l_argumentProcessedSuccessfully = true;
							l_inputCopy.erase(0, l_argumentEndPos + 1);
						}
						else
						{
							return DebugCommandErrorCode::ARGUMENT_PARSING;
						}

					}
					else
					{
						return DebugCommandErrorCode::ARGUMENT_PARSING;
					}
				}

				if (!l_argumentProcessedSuccessfully)
				{
					l_inputCopy.clear();
				}
			}
		}

		return DebugCommandErrorCode::OK;
	};


	void DebugCommand_handleError(DebugCommandErrorCode p_debugCommandErrorCode, std::string* out_errorMessage)
	{
		switch (p_debugCommandErrorCode)
		{
		case DebugCommandErrorCode::VERB_NOT_FOUND:
			out_errorMessage->assign("Verb is mendatory for a debug command. Example : 'print ...'");
			break;
		case DebugCommandErrorCode::ARGUMENT_PARSING:
			out_errorMessage->assign("Arguments cannot be parsed. Args format is -${Argname}=\"${ArgValue}\"");
			break;
		default:
			break;
		}
	};
}