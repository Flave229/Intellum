#include "FontSystem.h"
#include "UISystem.h"

FontSystem::FontSystem(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Box screenSize) : _direct3D(direct3D), _shaderController(shaderController), _fontEngine(fontEngine), _screenSize(screenSize)
{
}

FontSystem::~FontSystem()
{
}

void FontSystem::Shutdown()
{
}

void FontSystem::Update(vector<Entity*> entities, float delta)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(TEXT);

		if (component == nullptr)
			continue;

		TextComponent* text = static_cast<TextComponent*>(component);

		text->TextEntity = _fontEngine->ConvertTextToEntities(text->FontPosition, "Impact", text->Text, text->Color, text->FontSize);

		for (TextTexture characterTexture : text->TextEntity)
			UpdateAppearance(characterTexture);
	}
}

void FontSystem::UpdateAppearance(TextTexture texture)
{
	if ((texture.Position.x == static_cast<int>(texture.PreviousPosition.x)) && (texture.Position.y == static_cast<int>(texture.PreviousPosition.y))
		&& (texture.Size == texture.PreviousSize))
	{
		return;
	}

	texture.PreviousPosition = texture.Position;
	texture.PreviousSize = texture.Size;

	float left = ((_screenSize.Width / 2) * -1) + texture.Position.x;
	float right = left + texture.Size;

	float top = (_screenSize.Height / 2) - texture.Position.y;
	float bottom = top - texture.Size * 2;

	Vertex* vertices = new Vertex[texture.Model.VertexCount];
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
	HRESULT result = _direct3D->GetDeviceContext()->Map(texture.Model.VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) throw Exception("Failed to map vertex buffer to the Device Context");

	Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

	memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * texture.Model.VertexCount));

	_direct3D->GetDeviceContext()->Unmap(texture.Model.VertexBuffer, 0);

	delete[] vertices;
	vertices = nullptr;
}

void FontSystem::Render(vector<Entity*> entities)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(TEXT);

		if (component == nullptr)
			continue;

		TextComponent* text = static_cast<TextComponent*>(component);

		RenderCharacters(text->TextEntity);
	}
}

void FontSystem::RenderCharacters(vector<TextTexture> characters)
{
	for (TextTexture character : characters)
	{
		BuildBufferInformation(character);
		ShaderResources shaderResources = BuildShaderResources(character);

		FontShader* shader = static_cast<FontShader*>(_shaderController->GetShader(SHADER_UI));
		shader->SetColorOverload(true, shaderResources.ColorParameters.Color);
		shader->Render(character.Model.IndexCount, shaderResources, _direct3D->GetWorldMatrix(), _direct3D->GetOrthoMatrix());
	}
}

void FontSystem::BuildBufferInformation(TextTexture character) const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);
	_direct3D->TurnZBufferOff();

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &character.Model.VertexBuffer, &character.Model.VBStride, &character.Model.VBOffset);
	deviceContext->IASetIndexBuffer(character.Model.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ShaderResources FontSystem::BuildShaderResources(TextTexture character) const
{
	ShaderResources shaderResources = ShaderResources();
	shaderResources.MatrixParameters.WorldMatrix = _direct3D->GetWorldMatrix();
	shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetOrthoMatrix();

	shaderResources.ColorParameters = character.Color;

	if (character.CharacterTexture != nullptr)
		shaderResources.TextureParameters.TextureArray = vector<ID3D11ShaderResourceView*> { ExtractResourceViewsFrom(character.CharacterTexture) };

	return shaderResources;
}

ID3D11ShaderResourceView* FontSystem::ExtractResourceViewsFrom(Texture* texture)
{
	return texture->GetTexture();
}