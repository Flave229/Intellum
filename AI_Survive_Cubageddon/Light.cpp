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

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	_specularColor = XMFLOAT4(red, green, blue, alpha);
}	

void Light::SetDirection(float x, float y, float z)
{
	_direction = XMFLOAT3(x, y, z);
}

void Light::SetSpecularPower(float power)
{
	_specularPower = power;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return _ambientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
	return _diffuseColor;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return _specularColor;
}

XMFLOAT3 Light::GetDirection()
{
	return _direction;
}

float Light::GetSpecularPower()
{
	return _specularPower;
}
