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
	~CameraBuffer() override = default;
	
	void Shutdown() override;

	void SetShaderParameters(ShaderParameters parameters) override;
	void SetShaderParameters(int bufferIndex, ShaderResources shaderResources) override;
};