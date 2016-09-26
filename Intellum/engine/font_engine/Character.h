#pragma once
#include <string>

#include "../../Bitmap.h"

using namespace std;

class Character
{
public:
	string _name;
	string _unicode;
	Bitmap* _texture;
public:
	Character(string unicode, Bitmap* texture);
	Character(string name, string unicode, Bitmap* texture);
	~Character();
};