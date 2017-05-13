#include "Light.h"

Light::Light(): AmbientColor(0, 0, 0, 0), DiffuseColor(0, 0, 0, 0), SpecularColor(0, 0, 0, 0), Direction(0, 0, 0), SpecularPower(0)
{
}

Light::~Light()
{
}