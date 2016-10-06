#pragma once
#include <vector>
#include <string>

#include "../../error_handling/Exception.h"
#include "Character.h"
#include "UnicodeDictionary.h"

using namespace std;

class Font
{
public:
	string _fontName;
	vector<Character*> _characters;
public:
	Font();
	~Font();

	Character* GetCharacterByName(string name);
	Character* GetCharacterByUnicode(string unicode);
};
