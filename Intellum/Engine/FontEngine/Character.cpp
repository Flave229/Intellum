#include "Character.h"

Character::Character(string unicode, Texture* texture) : Unicode(unicode), TextTexture(texture)
{
}

Character::Character(string name, string unicode, Texture* texture) : Name(name), Unicode(unicode), TextTexture(texture)
{
}

Character::~Character()
{
}