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
	XMFLOAT3 _direction;

// Function Declarations
public:
	Light();
	Light(const Light&);
	~Light();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);

	XMFLOAT4 GetAmbientColor();
	XMFLOAT4 GetDiffuseColor();
	XMFLOAT3 GetDirection();
};

#endif