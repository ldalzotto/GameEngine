#include "DocumentationGeneration.h"

#include "Utils/File/XMLParser/XMLParser.h"
#include "DataStructures/String.h"
#include "DataStructures/StringAlgorithm.h"

namespace _Documentation
{
	char* TitleXMLTag = "Title";
	char* ParagraphXMLTag = "Paragraph";

	void GeneratePage(_Utils::XMLGraph* p_pageInputXML, _Core::String* out_pageStr)
	{
		_Utils::XMLGraphIterator l_pageIt = _Utils::XMLGraphIterator_build(p_pageInputXML);
		while (_Utils::XMLGraphIterator_moveNext(&l_pageIt))
		{
			_Utils::XMLNode* l_node = l_pageIt.CurrentNode;
			if (_Core::String_equals(&l_node->Name, TitleXMLTag))
			{
				//Title found
				int zdda = 0;
			}
		}
	};
}