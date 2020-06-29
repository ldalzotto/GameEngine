#pragma once

namespace _GameEngine
{
	namespace _Utils { struct XMLGraph; }
	namespace _Core { struct String; }
}

using namespace _GameEngine;

namespace _Documentation
{
	void GeneratePage(_Utils::XMLGraph* p_pageInputXML, _Core::String* out_pageStr);
}