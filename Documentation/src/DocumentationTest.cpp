#include "DocumentationTest.h"

#include "DataStructures/StringAlgorithm.h"
#include "Utils/File/XMLParser/XMLParser.h"

using namespace _GameEngine;

int main()
{
	_Core::String l_str;
	l_str.alloc(50);
	_Core::String_append(&l_str, "<Root> <Title>Bonjour</Title> <Section i=\"1\">Section 1</Section> </Root>");
	_Utils::XMLGraph l_xmlGraph;
	_Utils::XMLGraph_parse(&l_str, &l_xmlGraph);
	return 0;
}

namespace _Documentation
{

}