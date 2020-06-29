#pragma once

#include "DataStructures/String.h"
#include "DataStructures/VectorT.h"

namespace _GameEngine::_Utils
{
	struct XMLAttribute
	{
		_Core::String Name;
		_Core::String Value;
	};

	struct XMLNode
	{
		_Core::VectorT<XMLAttribute> Attributes;
		_Core::VectorT<XMLNode> Childs;

		_Core::String Name;
		_Core::String Content;

		size_t XmlBeginIndex;
		size_t XMlEndIndex;
	};

	struct XMLGraph
	{
		XMLNode RootNode;
	};

	void XMLGraph_parse(_Core::String* p_input, XMLGraph* p_outGraph);
}