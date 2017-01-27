#pragma once

#include <DirectXMath.h>
#include "../DirectX3D.h"

using namespace DirectX;

class Frustrum
{
private:
	DirectX3D* _direct3D;
	XMFLOAT4 _planes[6];

	static void CalculateMinimumZDistanceFrom(XMFLOAT4X4& projectionMatrix, float screenDepth);
	void ConstructFrustrumNearPlane(XMFLOAT4X4 matrix);
	void ConstructFrustrumFarPlane(XMFLOAT4X4 matrix);
	void ConstructFrustrumLeftPlane(XMFLOAT4X4 matrix);
	void ConstructFrustrumRightPlane(XMFLOAT4X4 matrix);
	void ConstructFrustrumTopPlane(XMFLOAT4X4 matrix);
	void ConstructFrustrumBottomPlane(XMFLOAT4X4 matrix);
public:
	Frustrum(DirectX3D* direct3D);
	Frustrum(const Frustrum& other);
	~Frustrum();
	void ConstructFrustrum(XMFLOAT4X4 viewMatrix, float screenDepth);

	bool CheckPointInsideFrustrum(XMFLOAT3 point, float minDistanceFromPlane) const;
	bool CheckCubeInsideFrustrum(XMFLOAT3 center, float cornerDistance) const;
	bool CheckSphereInsideFrustrum(XMFLOAT3 center, float radius) const;
	bool CheckRectangleInsideFrustrum(XMFLOAT3 center, XMFLOAT3 size) const;
};

