#include "Font.h"

Font::Font()
{
}


Font::~Font()
{
}

Character* Font::GetCharacterByName(string name)
{
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters.at(i)->_name != name)
			continue;

		return _characters.at(i);
	}

	return GetCharacterByUnicode("0000");
}

Character* Font::GetCharacterByUnicode(string unicode)
{
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters.at(i)->_unicode != unicode)
			continue;

		return _characters.at(i);
	}

	throw Exception("Could not find the unicode character \"" + unicode + "\"");
}