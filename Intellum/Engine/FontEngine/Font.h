#pragma once
#include <vector>
#include <string>

#include "../../ErrorHandling/Exception.h"
#include "Character.h"
#include "UnicodeDictionary.h"

using namespace std;

class Font
{
public:
	string _fontName;
	vector<Character> _characters;
public:
	Font();
	~Font();

	Texture* GetCharacterTextureByName(string name);
	Texture* GetCharacterTextureByUnicode(string unicode);
};
