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

	//TODO -> This structure can be generalized to any graph-like structure (with Parent <1-*> Child relationship )
	struct XMLNode
	{
		XMLNode* Parent;
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

	//TODO -> The same as XMLNode. This iterator can be generalized to any graph-like structure.
	struct XMLGraphIterator
	{
		XMLNode* RootNode;
		XMLNode* CurrentNode;
	};

	XMLGraphIterator XMLGraphIterator_build(XMLGraph* p_graph);
	XMLGraphIterator XMLGraphIterator_build(XMLNode* p_startNode);

	bool XMLGraphIterator_moveNext(XMLGraphIterator* p_graphIterator);
	bool XMLGraphIterator_skipCurrentChildrens(XMLGraphIterator* p_graphIterator);

	/**
	//TODO -> Maybe having a custom allocator for temporary objects ?
	*/
	void XMLGraph_parse(_Core::String* p_input, XMLGraph* p_outGraph);
	void XMLGraph_free(XMLGraph* p_xmlGraph);
}