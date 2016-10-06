#pragma once
#include <vector>
#include <string>
#include <map>

#include "../../error_handling/Exception.h"
#include "Character.h"

using namespace std;

class UnicodeDictionary
{
public:
	map<string, string> _unicodeToLetter = map<string, string>();
public:
	UnicodeDictionary();
	~UnicodeDictionary();
};
