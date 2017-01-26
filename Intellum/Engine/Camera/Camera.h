#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
#include "../Object/Transform.h"

using namespace DirectX;

class Camera
{
// Member Level Variables
private:
	Transform* _transform;

	XMMATRIX _ViewMatrix2D;
	XMMATRIX _viewMatrix;

// Function Declarations
public:
	Camera(Transform* transform);
	Camera(const Camera& other);
	~Camera();

	Transform* GetTransform() const;

	void Update(float delta);
	void MapViewMatrixInto(XMMATRIX& matrix) const;
};

#endif