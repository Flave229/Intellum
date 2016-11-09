#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
// Member Level Variables
private:
	float _positionX;
	float _positionY;
	float _positionZ;

	float _rotationX;
	float _rotationY;
	float _rotationZ;

	XMMATRIX _viewMatrix;

// Function Declarations
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void MapViewMatrixInto(XMMATRIX&);
};

#endif