#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
// Member Level Variables
private:
	XMFLOAT3 _position;
	XMFLOAT3 _rotation;

	XMMATRIX _viewMatrix;

// Function Declarations
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(XMFLOAT3 position);
	void SetRotation(XMFLOAT3 rotation);

	XMFLOAT3 GetPosition();
	XMFLOAT3 GetRotation();

	void Render();
	void MapViewMatrixInto(XMMATRIX&);
};

#endif