#include "Camera.h"

Camera::Camera(Transform* transform): _transform(transform)
{
	_transform->SetPosition(XMFLOAT3(0.0f, 0.0f, -1.0f));
	Update(0.0f);
}

Camera::Camera(const Camera& other): _transform(other._transform)
{
}

Camera::~Camera()
{
}

Transform* Camera::GetTransform() const
{
	return _transform;
}

void Camera::Update(float delta)
{
	_transform->SetAngularVelocity(XMFLOAT3(0.0f, 1.5f, 0.0f));
	_transform->Update(delta);

	XMFLOAT3 rotation = _transform->GetRotation();
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);

	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);
	
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	
	XMVECTOR positionVector = XMLoadFloat3(&_transform->GetPosition());
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::MapViewMatrixInto(XMMATRIX& viewMatrix) const
{
	viewMatrix = _viewMatrix;
}
