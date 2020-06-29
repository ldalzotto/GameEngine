#include "XMLParser.h"

#include "DataStructures/StringAlgorithm.h"
#include "DataStructures/ElementComparators.h"

namespace _GameEngine::_Utils
{
	short XMLNode_StartIndexOrdering(XMLNode* p_left, XMLNode* p_right, void* p_null)
	{
		return _Core::SizeTSortCompararator(p_left->XmlBeginIndex, p_right->XmlBeginIndex);
	}
}

namespace _GameEngine::_Utils
{
	_Core::FixedString CHAR_INFERIOR = _Core::FixedString_interpret("<");
	_Core::FixedString CHAR_SUPERIOR = _Core::FixedString_interpret(">");

	struct XMLBalise
	{
		bool IsStart;
		size_t BeginIndex;
		size_t EndIndex;
		_Core::String BaliseName;
	};

	void XMLBalise_allocFromString(XMLBalise* p_xmlBalise, _Core::String* p_baliseFullString, size_t p_beginIndex, size_t p_endIndex)
	{
		p_xmlBalise->IsStart = !(*p_baliseFullString->Vector.at(1) == '/');
		p_xmlBalise->BaliseName.alloc((p_endIndex - p_beginIndex));
		if (p_xmlBalise->IsStart)
		{
			_Core::String_substring(p_baliseFullString, 1, (p_endIndex - p_beginIndex) - 1, &p_xmlBalise->BaliseName);
		}
		else
		{
			_Core::String_substring(p_baliseFullString, 2, (p_endIndex - p_beginIndex) - 1, &p_xmlBalise->BaliseName);
		}

		p_xmlBalise->BeginIndex = p_beginIndex;
		p_xmlBalise->EndIndex = p_endIndex;
	};

	void XMLBalise_free(XMLBalise* p_xmlBalise, void* p_null)
	{
		p_xmlBalise->BaliseName.free();
	};

	bool XMLGraph_findNextBalise(_Core::String* p_input, size_t p_startIndex, XMLBalise* out_xmlBalise)
	{
		size_t l_nodeBeginXMLBaliseStart;
		if (STRING_FIND(p_input, &CHAR_INFERIOR, p_startIndex, &l_nodeBeginXMLBaliseStart))
		{
			size_t l_nodeBeginXMLBaliseEnd;
			if (STRING_FIND(p_input, &CHAR_SUPERIOR, l_nodeBeginXMLBaliseStart + 1, &l_nodeBeginXMLBaliseEnd))
			{
				_Core::String l_balise;
				l_balise.alloc(0);
				_Core::String_substring(p_input, l_nodeBeginXMLBaliseStart, l_nodeBeginXMLBaliseEnd, &l_balise);
				XMLBalise_allocFromString(out_xmlBalise, &l_balise, l_nodeBeginXMLBaliseStart, l_nodeBeginXMLBaliseEnd);
				return true;
			}
		}

		return false;
	}

	void XMLNode_alloc(XMLNode* p_node, XMLBalise* p_beging, XMLBalise* p_end, _Core::String* p_xml)
	{
		p_node->Name.alloc(p_beging->BaliseName.Vector.size());
		_Core::String_append(&p_node->Name, p_beging->BaliseName.c_str());

		p_node->Content.alloc(0);
		_Core::String_substring(p_xml, p_beging->EndIndex + 1, p_end->BeginIndex - 1, &p_node->Content);

		p_node->XmlBeginIndex = p_beging->BeginIndex;
		p_node->XMlEndIndex = p_end->EndIndex;

		p_node->Attributes.alloc();
		p_node->Childs.alloc();
	};

	void XMLNode_free(XMLNode* p_node)
	{
		p_node->Name.free();
		p_node->Content.free();
		p_node->Attributes.free();
		p_node->Childs.free();
	};

	void XMLGraph_parse(_Core::String* p_input, XMLGraph* p_outGraph)
	{
		size_t l_nodeStartIndex = 0;
		size_t l_nodeEndIndex = 0;

		_Core::VectorT<XMLBalise> XmlBalises;
		XmlBalises.alloc();
		{
			size_t l_xmlCursor = 0;
			XMLBalise l_balise{};
			while (XMLGraph_findNextBalise(p_input, l_xmlCursor, &l_balise))
			{
				XmlBalises.push_back(&l_balise);
				l_xmlCursor = l_balise.EndIndex;
			}



			_Core::VectorT<XMLNode> InstanciatedXMLNodeStack;
			InstanciatedXMLNodeStack.alloc();

			// XML Node instanciation
			{
				_Core::VectorT<size_t> XmlOpenBaliseIndicesStack;
				XmlOpenBaliseIndicesStack.alloc(XmlBalises.size());
				{
					for (size_t i = 0; i < XmlBalises.size(); i++)
					{
						XMLBalise* p_xmlBalise = XmlBalises.at(i);
						if (p_xmlBalise->IsStart)
						{
							XmlOpenBaliseIndicesStack.push_back(&i);
						}
						else
						{
							size_t l_beginBaliseArrayIndex = *XmlOpenBaliseIndicesStack.at(XmlOpenBaliseIndicesStack.size() - 1);

							XMLBalise* p_beginXMLBalise = XmlBalises.at(l_beginBaliseArrayIndex);
							XMLNode l_newNode{};
							XMLNode_alloc(&l_newNode, p_beginXMLBalise, p_xmlBalise, p_input);
							InstanciatedXMLNodeStack.push_back(&l_newNode);
							XmlOpenBaliseIndicesStack.erase(XmlOpenBaliseIndicesStack.size() - 1);
						}
					}
				}
				XmlOpenBaliseIndicesStack.free();
			}

			// XML Node hierarchy
			{
				InstanciatedXMLNodeStack.selectionSort(XMLNode_StartIndexOrdering, (void*)nullptr);

				for (size_t l_childIndex = InstanciatedXMLNodeStack.size() - 1; l_childIndex < InstanciatedXMLNodeStack.size(); l_childIndex--)
				{
					XMLNode* l_childNode = InstanciatedXMLNodeStack.at(l_childIndex);
					if (l_childIndex == 0)
					{
						p_outGraph->RootNode = *l_childNode;
					}
					else
					{
						for (size_t l_parentIndex = l_childIndex - 1; l_parentIndex < l_childIndex; l_parentIndex--)
						{
							XMLNode* l_potentialParentNode = InstanciatedXMLNodeStack.at(l_parentIndex);
							if (l_potentialParentNode->XMlEndIndex > l_childNode->XMlEndIndex)
							{
								l_potentialParentNode->Childs.push_back(l_childNode);
							}
						}
					}


				}
			}

			// Individual nodes are not disposed because they copied recursively to p_outGraph
			InstanciatedXMLNodeStack.free();

			InstanciatedXMLNodeStack.free();
		}
		XmlBalises.forEach(XMLBalise_free, (void*)nullptr);
		XmlBalises.free();


	};
}