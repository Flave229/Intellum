#include "Light.h"

Light::Light(): Transform(nullptr), AmbientColor(0, 0, 0, 0), DiffuseColor(0, 0, 0, 0), SpecularColor(0, 0, 0, 0), SpecularPower(0)
{
}

Light::~Light()
{
}

void Light::Shutdown()
{
	if (Transform != nullptr)
	{
		delete Transform;
		Transform = nullptr;
	}
}

void Light::Update(float delta)
{
	Transform->Update(delta);
}

XMFLOAT3 Light::GetDirection(XMMATRIX objectMatrix)
{
	XMFLOAT3& transform = Transform->GetPosition();
	XMFLOAT4X4 objectPosition;
	XMStoreFloat4x4(&objectPosition, objectMatrix);
	XMFLOAT3 lightDirection = XMFLOAT3(objectPosition._14 - transform.x, objectPosition._24 - transform.y, objectPosition._34 - transform.z);

	float magnitude = lightDirection.x + lightDirection.y + lightDirection.z;
	return XMFLOAT3(lightDirection.x / magnitude, lightDirection.y / magnitude, lightDirection.z / magnitude);
}