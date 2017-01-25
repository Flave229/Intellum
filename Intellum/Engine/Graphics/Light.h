#ifndef _LIGHT_H_
#define	_LIGHT_H_

#include <DirectXMath.h>

using namespace DirectX;

class Light
{
// Member Level Variables
private:
	XMFLOAT4 _ambientColor;
	XMFLOAT4 _diffuseColor;
	XMFLOAT4 _specularColor;

	XMFLOAT3 _direction;
	float _specularPower;

// Function Declarations
public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(XMFLOAT4 color);
	void SetDiffuseColor(XMFLOAT4 color);
	void SetSpecularColor(XMFLOAT4 color);

	void SetDirection(XMFLOAT3 direction);
	void SetSpecularPower(float power);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT4 GetSpecularColor();

	XMFLOAT3 GetDirection();
	float GetSpecularPower();
};

#endif