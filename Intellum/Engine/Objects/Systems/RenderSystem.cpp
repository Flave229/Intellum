#include "RenderSystem.h"
#include "../Components/RasterizerComponent.h"
#include "../Components/FurstrumCullingComponent.h"

RenderSystem::RenderSystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera) : _direct3D(direct3D), _camera(camera), _shaderController(shaderController), _renderCount(0)
{
}

void RenderSystem::Shutdown()
{
}

void RenderSystem::Update(vector<Entity*> entities, float delta)
{
	_renderCount = 0;
}

void RenderSystem::Render(vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(APPEARANCE);

		if (component == nullptr)
			continue;

		AppearanceComponent* appearance = static_cast<AppearanceComponent*>(component);

		component = entity->GetComponent(TRANSFORM);

		if (component == nullptr)
			continue;

		TransformComponent* transform = static_cast<TransformComponent*>(component);

		if (CheckIfInsideFrustrum(entity, transform, appearance) == false)
			continue;

		BuildBufferInformation(entity, appearance);

		ShaderResources shaderResources = ShaderResources();
		shaderResources.ColorOverload = appearance->Color;
		shaderResources.GradientOverload = appearance->Gradient;
		shaderResources.GradientOverload.CenterYCordinates = transform->Position.y;
		shaderResources.GradientOverload.Height = (appearance->Model.Size.y / 2) * transform->Scale.y;
		shaderResources.TextureArray = ExtractResourceViewsFrom(appearance->Textures);
		if (appearance->BumpMap != nullptr)
			shaderResources.BumpMap = appearance->BumpMap->GetTexture();
		if (appearance->LightMap != nullptr)
			shaderResources.LightMap = appearance->LightMap->GetTexture();

		IShaderType* shader = _shaderController->GetShader(appearance->ShaderType);
		switch(appearance->ShaderType)
		{
		case SHADER_DEFAULT:
			shader->Render(appearance->Model.IndexCount, shaderResources, transform->Transformation, _direct3D->GetProjectionMatrix());
			break;
		case SHADER_UI:
			shader->Render(appearance->Model.IndexCount, shaderResources, _direct3D->GetWorldMatrix(), _direct3D->GetOrthoMatrix());
			break;
		default:
			shader->Render(appearance->Model.IndexCount, shaderResources, transform->Transformation, _direct3D->GetProjectionMatrix());
		}
		
		_renderCount++;
	}
}

bool RenderSystem::CheckIfInsideFrustrum(Entity* entity, TransformComponent* transform, AppearanceComponent* appearance)
{
	IComponent* component = entity->GetComponent(FRUSTRUM_CULLING);

	if (component == nullptr)
		return true;

	FrustrumCullingComponent* frustrumCulling = static_cast<FrustrumCullingComponent*>(component);

	Frustrum* frustrum = _camera->GetFrustrum();

	XMFLOAT3 scaledSize = XMFLOAT3(appearance->Model.Size.x * transform->Scale.x, appearance->Model.Size.y * transform->Scale.y, appearance->Model.Size.z * transform->Scale.z);

	switch (frustrumCulling->CullingType)
	{
	case FRUSTRUM_CULL_POINT:
		return frustrum->CheckPointInsideFrustrum(transform->Position, 0.0f);
	case FRUSTRUM_CULL_RECTANGLE:
		return frustrum->CheckRectangleInsideFrustrum(transform->Position, scaledSize);
	case FRUSTRUM_CULL_SPHERE:
		return frustrum->CheckSphereInsideFrustrum(transform->Position, 0.5f * scaledSize.x);
	case FRUSTRUM_CULL_SQUARE:
		return frustrum->CheckCubeInsideFrustrum(transform->Position, 0.5f * scaledSize.x);
	default: 
		return true;
	}
}

void RenderSystem::BuildBufferInformation(Entity* entity, AppearanceComponent* appearance) const
{
	IComponent* component = entity->GetComponent(RASTERIZER);

	if (component == nullptr)
		_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);
	else
		_direct3D->GetRasterizer()->SetRasterizerCullMode(static_cast<RasterizerComponent*>(component)->CullMode);

	switch (appearance->ShaderType)
	{
	case SHADER_UI:
		_direct3D->TurnZBufferOff();
		break;
	default:
		_direct3D->TurnZBufferOn();
	}

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &appearance->Model.VertexBuffer, &appearance->Model.VBStride, &appearance->Model.VBOffset);
	deviceContext->IASetIndexBuffer(appearance->Model.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

vector<ID3D11ShaderResourceView*> RenderSystem::ExtractResourceViewsFrom(vector<Texture*> textures)
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}

int RenderSystem::RenderCount() const
{
	return _renderCount;
}