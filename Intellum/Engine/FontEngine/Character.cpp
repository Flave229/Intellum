#include "Character.h"

Character::Character(string unicode, Bitmap* texture) : _unicode(unicode), _texture(texture)
{
}

Character::Character(string name, string unicode, Bitmap* texture) : _name(name), _unicode(unicode), _texture(texture)
{
}

Character::~Character()
{
}
