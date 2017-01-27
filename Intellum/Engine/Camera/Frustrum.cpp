#include "Frustrum.h"

Frustrum::Frustrum(DirectX3D* direct3D): _direct3D(direct3D)
{
}

Frustrum::Frustrum(const Frustrum& other) : _direct3D(other._direct3D)
{
}

Frustrum::~Frustrum()
{
}

void Frustrum::ConstructFrustrum(XMFLOAT4X4 viewMatrix, float screenDepth)
{
	XMFLOAT4X4 projectionMatrix;
	XMStoreFloat4x4(&projectionMatrix, _direct3D->GetProjectionMatrix());

	CalculateMinimumZDistanceFrom(projectionMatrix, screenDepth);

	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, XMMatrixMultiply(XMLoadFloat4x4(&viewMatrix), XMLoadFloat4x4(&projectionMatrix)));

	ConstructFrustrumNearPlane(matrix);
	ConstructFrustrumFarPlane(matrix);
	ConstructFrustrumLeftPlane(matrix);
	ConstructFrustrumRightPlane(matrix);
	ConstructFrustrumTopPlane(matrix);
	ConstructFrustrumBottomPlane(matrix);
}

void Frustrum::CalculateMinimumZDistanceFrom(XMFLOAT4X4& projectionMatrix, float screenDepth)
{
	float zMinimum = -projectionMatrix._43 / projectionMatrix._33;
	float frustrumRadius = screenDepth / (screenDepth - zMinimum);

	projectionMatrix._33 = frustrumRadius;
	projectionMatrix._43 = -frustrumRadius * zMinimum;
}

void Frustrum::ConstructFrustrumNearPlane(XMFLOAT4X4 matrix)
{
	_planes[0].x = matrix._14 + matrix._13;
	_planes[0].y = matrix._24 + matrix._23;
	_planes[0].z = matrix._34 + matrix._33;
	_planes[0].w = matrix._44 + matrix._43;
	XMStoreFloat4(&_planes[0], XMPlaneNormalize(XMLoadFloat4(&_planes[0])));
}

void Frustrum::ConstructFrustrumFarPlane(XMFLOAT4X4 matrix)
{
	_planes[1].x = matrix._14 - matrix._13;
	_planes[1].y = matrix._24 - matrix._23;
	_planes[1].z = matrix._34 - matrix._33;
	_planes[1].w = matrix._44 - matrix._43;
	XMStoreFloat4(&_planes[1], XMPlaneNormalize(XMLoadFloat4(&_planes[1])));
}

void Frustrum::ConstructFrustrumLeftPlane(XMFLOAT4X4 matrix)
{
	_planes[2].x = matrix._14 + matrix._11;
	_planes[2].y = matrix._24 + matrix._21;
	_planes[2].z = matrix._34 + matrix._31;
	_planes[2].w = matrix._44 + matrix._41;
	XMStoreFloat4(&_planes[2], XMPlaneNormalize(XMLoadFloat4(&_planes[2])));
}

void Frustrum::ConstructFrustrumRightPlane(XMFLOAT4X4 matrix)
{
	_planes[3].x = matrix._14 - matrix._11;
	_planes[3].y = matrix._24 - matrix._21;
	_planes[3].z = matrix._34 - matrix._31;
	_planes[3].w = matrix._44 - matrix._41;
	XMStoreFloat4(&_planes[3], XMPlaneNormalize(XMLoadFloat4(&_planes[3])));
}

void Frustrum::ConstructFrustrumTopPlane(XMFLOAT4X4 matrix)
{
	_planes[4].x = matrix._14 - matrix._12;
	_planes[4].y = matrix._24 - matrix._22;
	_planes[4].z = matrix._34 - matrix._32;
	_planes[4].w = matrix._44 - matrix._42;
	XMStoreFloat4(&_planes[4], XMPlaneNormalize(XMLoadFloat4(&_planes[4])));
}

void Frustrum::ConstructFrustrumBottomPlane(XMFLOAT4X4 matrix)
{
	_planes[5].x = matrix._14 + matrix._12;
	_planes[5].y = matrix._24 + matrix._22;
	_planes[5].z = matrix._34 + matrix._32;
	_planes[5].w = matrix._44 + matrix._42;
	XMStoreFloat4(&_planes[5], XMPlaneNormalize(XMLoadFloat4(&_planes[5])));
}

bool Frustrum::CheckPointInsideFrustrum(XMFLOAT3 point, float minDistanceFromPlane) const
{
	for (int i = 0; i < 6; i++)
	{
		XMFLOAT4 planeDotCoords;
		XMStoreFloat4(&planeDotCoords, XMPlaneDotCoord(XMLoadFloat4(&_planes[i]), XMLoadFloat3(&point)));

		if(planeDotCoords.x < minDistanceFromPlane)
		{
			return false;
		}
	}

	return true;
}

bool Frustrum::CheckCubeInsideFrustrum(XMFLOAT3 center, float radius) const
{
	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - radius, center.y - radius, center.z - radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + radius, center.y - radius, center.z - radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - radius, center.y + radius, center.z - radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + radius, center.y + radius, center.z - radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - radius, center.y - radius, center.z + radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + radius, center.y - radius, center.z + radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - radius, center.y + radius, center.z + radius), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + radius, center.y + radius, center.z + radius), 0.0f) == false)
		return false;

	return true;
}

bool Frustrum::CheckSphereInsideFrustrum(XMFLOAT3 center, float radius) const
{
	return CheckPointInsideFrustrum(XMFLOAT3(center.x, center.y, center.z), -radius);
}

bool Frustrum::CheckRectangleInsideFrustrum(XMFLOAT3 center, XMFLOAT3 size) const
{
	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - size.x, center.y - size.y, center.z - size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + size.x, center.y - size.y, center.z - size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - size.x, center.y + size.y, center.z - size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + size.x, center.y + size.y, center.z - size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - size.x, center.y - size.y, center.z + size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + size.x, center.y - size.y, center.z + size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x - size.x, center.y + size.y, center.z + size.z), 0.0f) == false)
		return false;

	if (CheckPointInsideFrustrum(XMFLOAT3(center.x + size.x, center.y + size.y, center.z + size.z), 0.0f) == false)
		return false;

	return true;
}