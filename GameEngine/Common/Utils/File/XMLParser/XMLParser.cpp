#include "XMLParser.h"

#include "DataStructures/StringAlgorithm.h"

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

	void XMLNode_allocXMLNode(XMLNode* p_node, XMLBalise* p_beging, XMLBalise* p_end, _Core::String* p_xml)
	{
		p_node->Content.alloc(0);
		_Core::String_substring(p_xml, p_beging->EndIndex + 1, p_end->BeginIndex - 1, &p_node->Content);
	}

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

						XMLBalise* p_beingXMLBalise = XmlBalises.at(l_beginBaliseArrayIndex);

						XMLNode l_node;
						XMLNode_allocXMLNode(&l_node, p_beingXMLBalise, p_xmlBalise, p_input);

						XmlOpenBaliseIndicesStack.erase(XmlOpenBaliseIndicesStack.size() - 1);
					}
				}
			}
			XmlOpenBaliseIndicesStack.free();
		}
		XmlBalises.forEach(XMLBalise_free, (void*)nullptr);
		XmlBalises.free();

		
	};
}