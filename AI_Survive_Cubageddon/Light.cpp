#include "Light.h"

Light::Light()
{
}

Light::Light(const Light& other)
{
}

Light::~Light()
{
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	_ambientColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	_diffuseColor = XMFLOAT4(red, green, blue, alpha);
}

void Light::SetDirection(float x, float y, float z)
{
	_direction = XMFLOAT3(x, y, z);
}

XMFLOAT4 Light::GetAmbientColor()
{
	return _ambientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return _diffuseColor;
}

XMFLOAT3 Light::GetDirection()
{
	return _direction;
}
