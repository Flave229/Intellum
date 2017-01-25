#pragma once
#include <vector>
#include <string>
#include <map>

#include "../../ErrorHandling/Exception.h"
#include "Character.h"

using namespace std;

class UnicodeDictionary
{
public:
	map<string, string> _unicodeToLetter = map<string, string>();
public:
	UnicodeDictionary();
	~UnicodeDictionary();

	vector<string> GetSupportedUnicodeList();
	string GetCharacterForUnicode(string unicode);
};
