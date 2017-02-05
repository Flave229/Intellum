#pragma once
#include <DirectXMath.h>
#include "IShaderBuffer.h"
#include "../../Camera/Camera.h"

using namespace DirectX;

class CameraBuffer : public IShaderBuffer
{
private:
	struct Buffer
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	ID3D11Buffer* _buffer;
	Camera* _camera;
private:
	void Initialise() override;

public:
	CameraBuffer(DirectX3D* direct3D, Camera* camera);
	~CameraBuffer();
	
	void Shutdown() override;

	void SetShaderParameters(XMMATRIX worldMatrix, XMMATRIX projectionMatrix, XMMATRIX viewMatrix, int bufferIndex) override;
};