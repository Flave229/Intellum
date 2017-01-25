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

void Light::SetAmbientColor(XMFLOAT4 color)
{
	_ambientColor = color;
}

void Light::SetDiffuseColor(XMFLOAT4 color)
{
	_diffuseColor = color;
}

void Light::SetSpecularColor(XMFLOAT4 color)
{
	_specularColor = color;
}	

void Light::SetDirection(XMFLOAT3 direction)
{
	_direction = direction;
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
