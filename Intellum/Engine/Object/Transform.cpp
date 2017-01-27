#include "Transform.h"

Transform::Transform(DirectX3D* direct3D) : _direct3D(direct3D), _position(XMFLOAT3(0.0f, 0.0f, 0.0f)), _rotation(XMFLOAT3(0.0f, 0.0f, 0.0f))
{
}

Transform::Transform(const Transform& other) : _direct3D(other._direct3D), _transformation(other._transformation), _position(other._position), _rotation(other._rotation)
{
}

Transform::~Transform()
{
}

void Transform::Update(float delta)
{
	XMMATRIX transformation;
	_direct3D->MapWorldMatrixInto(transformation);

	UpdateRotation(delta);

	transformation *= XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	transformation *= XMMatrixTranslation(_position.x, _position.y, _position.z);

	_transformation = transformation;
}

void Transform::UpdateRotation(float delta)
{
	_rotation.x = CapRotationRange(_rotation.x + _angularVelocity.x * delta);
	_rotation.y = CapRotationRange(_rotation.y + _angularVelocity.y * delta);
	_rotation.z = CapRotationRange(_rotation.z + _angularVelocity.z * delta);
}

XMMATRIX& Transform::GetTransformation()
{
	return _transformation;
}

void Transform::SetPosition(XMFLOAT3 position)
{
	_position = position;
}

XMFLOAT3& Transform::GetPosition()
{
	return _position;
}

void Transform::SetRotation(XMFLOAT3 rotation)
{
	CapRotationRange(rotation.x);
	CapRotationRange(rotation.y); 
	CapRotationRange(rotation.z);
	_rotation = rotation;
}

XMFLOAT3& Transform::GetRotation()
{
	return _rotation;
}

void Transform::SetAngularVelocity(XMFLOAT3 angularVelocity)
{
	_angularVelocity = angularVelocity;
}

XMFLOAT3& Transform::GetAngularVelocity()
{
	return _angularVelocity;
}

float Transform::CapRotationRange(float rotation)
{
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}
	else if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	return rotation;
}

void Transform::Move(XMFLOAT3 movementVector)
{
	_position = XMFLOAT3(_position.x + movementVector.x, _position.y + movementVector.y, _position.z + movementVector.z);
}
