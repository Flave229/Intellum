#pragma once
#include <string>

#include "../Graphics/Bitmap.h"

using namespace std;

class Character
{
public:
	string _name;
	string _unicode;
	Bitmap* BitmapTexture;
	Texture* Texture;
public:
	Character(string unicode, Bitmap* texture);
	Character(string name, string unicode, Bitmap* texture);
	~Character();
};