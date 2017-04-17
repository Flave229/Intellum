#pragma once
#include <string>
#include "../Objects/Texture/Texture.h"

using namespace std;

class Character
{
public:
	string Name;
	string Unicode;
	Texture* TextTexture;
public:
	Character(string unicode, Texture* texture);
	Character(string name, string unicode, Texture* texture);
	~Character();
};