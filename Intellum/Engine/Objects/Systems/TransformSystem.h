#pragma once
#include <vector>
#include "../../DirectX3D.h"
#include "../Entity.h"
#include "ISystem.h"

class TransformSystem : public ISystem
{
private:
	DirectX3D* _direct3D;

	static void UpdatePosition(XMFLOAT3& position, XMFLOAT3 velocity, float delta);
	static void UpdateRotation(XMFLOAT3& rotation, XMFLOAT3 velocity, float delta);
	static float CapRotationRange(float rotation);
public:
	TransformSystem(DirectX3D* direct3D);
	~TransformSystem() override = default;
	void Shutdown() override;
	
	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
};
