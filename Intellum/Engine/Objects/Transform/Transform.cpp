#include "Transform.h"

Transform::Transform(DirectX3D* direct3D) : _direct3D(direct3D), _position(XMFLOAT3(0.0f, 0.0f, 0.0f)), _rotation(XMFLOAT3(0.0f, 0.0f, 0.0f)), _scale(XMFLOAT3(1.0f, 1.0f, 1.0f))
{
}

Transform::~Transform()
{
}

void Transform::Update(float delta)
{
	XMMATRIX transformation = _direct3D->GetWorldMatrix();
	
	UpdatePosition(delta);
	UpdateRotation(delta);

	transformation *= XMMatrixScaling(_scale.x, _scale.y, _scale.z);
	transformation *= XMMatrixRotationRollPitchYaw(_rotation.x, _rotation.y, _rotation.z);
	transformation *= XMMatrixTranslation(_position.x, _position.y, _position.z);

	_transformation = transformation;
}

void Transform::UpdatePosition(float delta)
{
	_position.x += _velocity.x * delta;
	_position.y += _velocity.y * delta;
	_position.z += _velocity.z * delta;
}

void Transform::UpdateRotation(float delta)
{
	SetRotation(XMFLOAT3(_rotation.x + _angularVelocity.x * delta, _rotation.y + _angularVelocity.y * delta, _rotation.z + _angularVelocity.z * delta));
}

XMMATRIX& Transform::GetTransformation()
{
	return _transformation;
}

void Transform::SetScale(XMFLOAT3 scale)
{
	_scale = scale;
}

XMFLOAT3& Transform::GetScale()
{
	return _scale;
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

void Transform::SetVelocity(XMFLOAT3 velocity)
{
	_velocity = velocity;
}

void Transform::AddVelocity(XMFLOAT3 velocity)
{
	_velocity.x += velocity.x;
	_velocity.y += velocity.y;
	_velocity.z += velocity.z;
}

XMFLOAT3& Transform::GetVelocity()
{
	return _velocity;
}

void Transform::SetAngularVelocity(XMFLOAT3 angularVelocity)
{
	_angularVelocity = angularVelocity;
}

void Transform::AddAngularVelocity(XMFLOAT3 angularVelocity)
{
	_angularVelocity.x += angularVelocity.x;
	_angularVelocity.y += angularVelocity.y;
	_angularVelocity.z += angularVelocity.z;
}

XMFLOAT3& Transform::GetAngularVelocity()
{
	return _angularVelocity;
}

float Transform::CapRotationRange(float rotation)
{
	if (rotation > XM_2PI)
	{
		rotation -= XM_2PI;
	}
	else if (rotation < 0.0f)
	{
		rotation += XM_2PI;
	}

	return rotation;
}

void Transform::Move(XMFLOAT3 movementVector)
{
	_position = XMFLOAT3(_position.x + movementVector.x, _position.y + movementVector.y, _position.z + movementVector.z);
}
