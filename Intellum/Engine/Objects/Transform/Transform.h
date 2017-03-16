#pragma once
#include <DirectXMath.h>
#include "../../DirectX3D.h"

using namespace DirectX;

class Transform
{
private:
	DirectX3D* _direct3D;
	XMMATRIX _transformation;

	XMFLOAT3 _position;
	XMFLOAT3 _rotation;

	XMFLOAT3 _velocity;
	XMFLOAT3 _angularVelocity;

	void UpdatePosition(float delta);
	void UpdateRotation(float delta);
	static float CapRotationRange(float rotation);

public:
	Transform(DirectX3D* direct3D);
	Transform(const Transform& other);
	~Transform();

	void Update(float delta);
	
	XMMATRIX& GetTransformation();
	
	void SetPosition(XMFLOAT3 position);
	XMFLOAT3& GetPosition();

	void SetRotation(XMFLOAT3 rotation);
	XMFLOAT3& GetRotation();

	void SetVelocity(XMFLOAT3 velocity);
	void AddVelocity(XMFLOAT3 velocity);
	XMFLOAT3& GetVelocity();

	void SetAngularVelocity(XMFLOAT3 angularVelocity);
	void AddAngularVelocity(XMFLOAT3 angularVelocity);
	XMFLOAT3& GetAngularVelocity();

	void Move(XMFLOAT3 movementVector);
};