#pragma once
#include <vector>
#include <string>

#include "Character.h"

using namespace std;

class Font
{
public:
	string _fontName;
	vector<Character*> _characters;
public:
	Font();
	~Font();
};
