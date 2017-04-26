#include "Font.h"

Font::Font()
{
}

Font::~Font()
{
}

Texture* Font::GetCharacterTextureByName(string name)
{
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters.at(i).Name != name)
			continue;

		return _characters.at(i).TextTexture;
	}

	return GetCharacterTextureByUnicode("0000");
}

Texture* Font::GetCharacterTextureByUnicode(string unicode)
{
	for (int i = 0; i < _characters.size(); i++)
	{
		if (_characters.at(i).Unicode != unicode)
			continue;

		return _characters.at(i).TextTexture;
	}

	throw Exception("Could not find the unicode character \"" + unicode + "\"");
}