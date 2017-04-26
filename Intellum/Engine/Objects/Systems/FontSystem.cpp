#include "FontSystem.h"
#include "UISystem.h"

FontSystem::FontSystem(DirectX3D* direct3D, ShaderController* shaderController, FontEngine* fontEngine, HWND hwnd, Box screenSize) : _direct3D(direct3D), _shaderController(shaderController), _fontEngine(fontEngine), _uiSystem(direct3D, shaderController, hwnd, screenSize)
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

		if (text->Text == text->PreviousText)
			continue;

		text->TextEntity = _fontEngine->ConvertTextToEntities(text->FontPosition, "Impact", text->Text, text->Color, text->FontSize);

		_uiSystem.Update(text->TextEntity, 0);
	}
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

		for (unsigned long long i = text->TextEntity.size(); i > 0; i--)
		{
			text->TextEntity.back()->Shutdown();
			delete text->TextEntity.back();
			text->TextEntity.pop_back();
		}
	}
}

void FontSystem::RenderCharacters(vector<Entity*> entities)
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

		BuildBufferInformation(entity, appearance);

		ShaderResources shaderResources = BuildShaderResources(appearance, transform);

		FontShader* shader = static_cast<FontShader*>(_shaderController->GetShader(appearance->ShaderType));
		shader->SetColorOverload(true, shaderResources.ColorParameters.Color);
		shader->Render(appearance->Model.IndexCount, shaderResources, _direct3D->GetWorldMatrix(), _direct3D->GetOrthoMatrix());
	}
}

void FontSystem::BuildBufferInformation(Entity* entity, AppearanceComponent* appearance) const
{
	_direct3D->GetRasterizer()->SetRasterizerCullMode(D3D11_CULL_BACK);
	_direct3D->TurnZBufferOff();

	ID3D11DeviceContext* deviceContext = _direct3D->GetDeviceContext();
	deviceContext->IASetVertexBuffers(0, 1, &appearance->Model.VertexBuffer, &appearance->Model.VBStride, &appearance->Model.VBOffset);
	deviceContext->IASetIndexBuffer(appearance->Model.IndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ShaderResources FontSystem::BuildShaderResources(AppearanceComponent* appearance, TransformComponent* transform) const
{
	ShaderResources shaderResources = ShaderResources();
	shaderResources.MatrixParameters.WorldMatrix = _direct3D->GetWorldMatrix();
	shaderResources.MatrixParameters.ProjectionMatrix = _direct3D->GetOrthoMatrix();

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

vector<ID3D11ShaderResourceView*> FontSystem::ExtractResourceViewsFrom(vector<Texture*> textures)
{
	vector<ID3D11ShaderResourceView*> textureViews;

	for (Texture* texture : textures)
	{
		textureViews.push_back(texture->GetTexture());
	}

	return textureViews;
}