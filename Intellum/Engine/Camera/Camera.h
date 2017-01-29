#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include "../Object/Transform.h"
#include "Frustrum.h"
#include "../../Common/Constants.h"
#include "../Input/Input.h"

using namespace DirectX;

class Camera
{
// Member Level Variables
private:
	Input* _input;

	Frustrum* _frustrum;
	Transform* _transform;

	XMMATRIX _viewMatrix2D;
	XMMATRIX _viewMatrix;

// Function Declarations
public:
	Camera(Frustrum* frustrum, Transform* transform, Input* input);
	~Camera();

	void Shutdown();

	void Update(float delta);

	Frustrum* GetFrustrum() const;
	Transform* GetTransform() const;

	XMMATRIX GetViewMatrix() const;
};

#endif