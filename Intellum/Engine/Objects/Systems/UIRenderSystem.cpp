#include "UIRenderSystem.h"
#include "../../../common/Vertex.h"

UIRenderSystem::UIRenderSystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Camera* camera, Box screenSize): _direct3D(direct3D), _camera(camera), _shaderController(shaderController), _screenSize(screenSize)
{

}

void UIRenderSystem::Shutdown()
{
}

void UIRenderSystem::Update(vector<Entity*> entities, float delta)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(UI_APPEARANCE);

		if (component == nullptr)
			continue;

		UIAppearanceComponent* uiAppearance = static_cast<UIAppearanceComponent*>(component);

		if ((uiAppearance->Position.x == static_cast<int>(uiAppearance->PreviousPosition.x)) && (uiAppearance->Position.y == static_cast<int>(uiAppearance->PreviousPosition.y))
			&& (uiAppearance->BitmapSize.x == uiAppearance->PreviousBitmapSize.x) && (uiAppearance->BitmapSize.y == uiAppearance->PreviousBitmapSize.y))
		{
			return;
		}

		uiAppearance->PreviousPosition = uiAppearance->Position;
		uiAppearance->PreviousBitmapSize = uiAppearance->BitmapSize;

		float left = ((_screenSize.Width / 2) * -1) + (uiAppearance->Position.x);
		float right = left + uiAppearance->BitmapSize.x;

		float top = (_screenSize.Height / 2) - uiAppearance->Position.y;
		float bottom = top - uiAppearance->BitmapSize.y;

		Vertex* vertices = new Vertex[uiAppearance->Model.VertexCount];
		if (!vertices) throw Exception("Failed to initialise vertices for bitmap");

		vertices[0].position = XMFLOAT3(left, top, 0.0f);
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = XMFLOAT3(right, bottom, 0.0f);
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

		vertices[2].position = XMFLOAT3(left, bottom, 0.0f);
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[3].position = XMFLOAT3(left, top, 0.0f);
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[4].position = XMFLOAT3(right, top, 0.0f);
		vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[5].position = XMFLOAT3(right, bottom, 0.0f);
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = _direct3D->GetDeviceContext()->Map(uiAppearance->Model.VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) throw Exception("Failed to map vertex buffer to the Device Context");

		Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

		memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * uiAppearance->Model.VertexCount));

		_direct3D->GetDeviceContext()->Unmap(uiAppearance->Model.VertexBuffer, 0);

		delete[] vertices;
		vertices = nullptr;
	}
}

void UIRenderSystem::Render(vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
		_direct3D->TurnZBufferOff();

		IComponent* component = entity->GetComponent(UI_APPEARANCE);

		if (component == nullptr)
			continue;

		UIAppearanceComponent* appearance = static_cast<UIAppearanceComponent*>(component);
		
		BuildBufferInformation(entity, appearance);

		ShaderResources shaderResources = ShaderResources();
		shaderResources.textureArray = ExtractResourceViewsFrom(appearance->Textures);
		if (appearance->BumpMap != nullptr)
			shaderResources.bumpMap = appearance->BumpMap->GetTexture();
		if (appearance->LightMap != nullptr)
			shaderResources.lightMap = appearance->LightMap->GetTexture();

		IShaderType* shader = _shaderController->GetShader(appearance->ShaderType);
		shader->Render(appearance->Model.IndexCount, shaderResources, _direct3D->GetWorldMatrix(), _direct3D->GetOrthoMatrix());

		_direct3D->TurnZBufferOn();
	}
}

void UIRenderSystem::BuildBufferInformation(Entity* entity, UIAppearanceComponent* appearance) const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);

	unsigned int stride = sizeof(Vertex);
	unsigned int offset = 0;

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &appearance->Model.VertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(appearance->Model.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

vector<ID3D11ShaderResourceView*> UIRenderSystem::ExtractResourceViewsFrom(vector<Texture*> textures)
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}