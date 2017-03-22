#pragma once
#include <vector>
#include "../../DirectX3D.h"
#include "../Entity.h"

class TransformSystem
{
private:
	DirectX3D* _direct3D;

	static void UpdatePosition(XMFLOAT3& position, XMFLOAT3 velocity, float delta);
	static void UpdateRotation(XMFLOAT3& rotation, XMFLOAT3 velocity, float delta);
	static float CapRotationRange(float rotation);
public:
	TransformSystem(DirectX3D* direct3D);
	~TransformSystem();

	void Update(vector<Entity*> entities, float delta) const;
};