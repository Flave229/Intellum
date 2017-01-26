#include "Camera.h"

Camera::Camera(): _position(XMFLOAT3(0.0f, 0.0f, 0.0f)), _rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
{
}

Camera::Camera(const Camera& other): _position(other._position), _rotation(other._rotation)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(XMFLOAT3 position)
{
	_position = position;
}

void Camera::SetRotation(XMFLOAT3 rotation)
{
	_rotation = rotation;
}

XMFLOAT3 Camera::GetPosition()
{
	return _position;
}

XMFLOAT3 Camera::GetRotation()
{
	return _rotation;
}

void Camera::Render()
{
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&_position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	float yaw = _rotation.y * 0.0174532925f;
	float pitch = _rotation.x * 0.0174532925f;
	float roll = _rotation.z * 0.0174532925f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform LookAt and Up vector so the view is rotated correctly around origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate camera to the location of the viewer
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::MapViewMatrixInto(XMMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}
