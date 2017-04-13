#include "Character.h"

Character::Character(string unicode, Bitmap* texture) : _unicode(unicode), BitmapTexture(texture), Texture(texture->GetTexture())
{
}

Character::Character(string name, string unicode, Bitmap* texture) : _name(name), _unicode(unicode), BitmapTexture(texture), Texture(texture->GetTexture())
{
}

Character::~Character()
{
}