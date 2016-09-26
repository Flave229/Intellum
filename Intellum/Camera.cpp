#include "Camera.h"

Camera::Camera(): _positionX(0), _positionY(0), _positionZ(0), _rotationX(0), _rotationY(0), _rotationZ(0)
{
}

Camera::Camera(const Camera& other): _positionX(other._positionX), _positionY(other._positionY), _positionZ(other._positionY), 
										_rotationX(other._positionY), _rotationY(other._positionY), _rotationZ(other._positionY)
{
}

Camera::~Camera()
{
}

void Camera::SetPosition(float x, float y, float z)
{
	_positionX = x;
	_positionY = y;
	_positionZ = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	_rotationX = x;
	_rotationY = y;
	_rotationZ = z;
}

XMFLOAT3 Camera::GetPosition()
{
	return XMFLOAT3(_positionX, _positionY, _positionZ);
}

XMFLOAT3 Camera::GetRotation()
{
	return XMFLOAT3(_rotationX, _rotationY, _rotationZ);
}

void Camera::Render()
{
	XMFLOAT3 up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3 position = XMFLOAT3(_positionX, _positionY, _positionZ);
	XMFLOAT3 lookAt = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR positionVector = XMLoadFloat3(&position);
	XMVECTOR lookAtVector = XMLoadFloat3(&lookAt);
	float yaw = _rotationY * 0.0174532925f;
	float pitch = _rotationX * 0.0174532925f;
	float roll = _rotationZ * 0.0174532925f;
	XMMATRIX rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform LookAt and Up vector so the view is rotated correctly around origin.
	lookAtVector = XMVector3TransformCoord(lookAtVector, rotationMatrix);
	upVector = XMVector3TransformCoord(upVector, rotationMatrix);

	// Translate camera to the location of the viewer
	lookAtVector = XMVectorAdd(positionVector, lookAtVector);

	_viewMatrix = XMMatrixLookAtLH(positionVector, lookAtVector, upVector);
}

void Camera::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = _viewMatrix;
}
