#include "RenderSystem.h"
#include "../Components/RasterizerComponent.h"
#include "../Components/FurstrumCullingComponent.h"

RenderSystem::RenderSystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera) : _direct3D(direct3D), _camera(camera), _shaderController(shaderController), _renderCount(0)
{
	_defaultViewMatrix = camera->GetViewMatrix();
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

		ShaderResources shaderResources = BuildShaderResources(appearance, transform);

		IShaderType* shader = _shaderController->GetShader(appearance->ShaderType);
		shader->Render(appearance->Model.IndexCount, shaderResources);
		
		_renderCount++;
	}

	for (int i = 0; i < Observers.size(); i++)
	{
		ObserverEvent observerEvent;
		observerEvent.EventType = RENDER_COUNT;
		observerEvent.SetObservableData(_renderCount);
		Observers.at(i)->Notify(observerEvent);
		observerEvent.Shutdown<int>();
	}
}

bool RenderSystem::CheckIfInsideFrustrum(Entity* entity, TransformComponent* transform, AppearanceComponent* appearance) const
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

ShaderResources RenderSystem::BuildShaderResources(AppearanceComponent* appearance, TransformComponent* transform) const
{
	ShaderResources shaderResources = ShaderResources();
	IShaderType* shader = _shaderController->GetShader(appearance->ShaderType);
	switch (appearance->ShaderType)
	{
	case SHADER_DEFAULT:
		shaderResources.MatrixParameters.ViewMatrix = _camera->GetViewMatrix();
		shaderResources.MatrixParameters.WorldMatrix = transform->Transformation;
		shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetProjectionMatrix();
		break;
	case SHADER_UI:
	case SHADER_FONT:
		shaderResources.MatrixParameters.ViewMatrix = _defaultViewMatrix;
		shaderResources.MatrixParameters.WorldMatrix = _direct3D->GetWorldMatrix();
		shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetOrthoMatrix();
		break;
	default:
		shaderResources.MatrixParameters.ViewMatrix = _camera->GetViewMatrix();
		shaderResources.MatrixParameters.WorldMatrix = transform->Transformation;
		shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetProjectionMatrix();
	}

	shaderResources.ColorParameters = appearance->Color;
	shaderResources.GradientParameters = appearance->Gradient;
	if (appearance->Gradient.Enabled)
	{
		shaderResources.GradientParameters.CenterYCordinates = transform->Position.y;
		shaderResources.GradientParameters.Height = (appearance->Model.Size.y / 2) * transform->Scale.y;
	}

	if (appearance->Textures.size() > 0)
		shaderResources.TextureParameters.TextureArray = ExtractResourceViewsFrom(appearance->Textures);

	if (appearance->BumpMap != nullptr)
	{
		shaderResources.TextureParameters.BumpMap = appearance->BumpMap->GetTexture();
		shaderResources.TextureParameters.BumpMapEnabled = true;
	}

	if (appearance->LightMap != nullptr)
	{
		shaderResources.TextureParameters.LightMap = appearance->LightMap->GetTexture();
		shaderResources.TextureParameters.LightMapEnabled = true;
	}

	return shaderResources;
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
	case SHADER_FONT:
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

void RenderSystem::AddObserver(IObserver* observer)
{
	Observers.push_back(observer);
}