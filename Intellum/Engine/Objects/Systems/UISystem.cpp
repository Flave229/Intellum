#include "UISystem.h"
#include "../Components/TransformComponent.h"
#include "../Components/AppearanceComponent.h"

UISystem::UISystem(DirectX3D* direct3D, ShaderController* shaderController, HWND hwnd, Box screenSize): _direct3D(direct3D), _shaderController(shaderController), _screenSize(screenSize)
{

}

void UISystem::Shutdown()
{
}

void UISystem::Update(vector<Entity*>& entities, float delta)
{
	for(int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities.at(i);
		IComponent* component = entity->GetComponent(APPEARANCE);

		if (component == nullptr)
			continue;

		AppearanceComponent* appearance = static_cast<AppearanceComponent*>(component);

		component = entity->GetComponent(USER_INTERFACE);

		if (component == nullptr)
			continue;

		UIComponent* ui = static_cast<UIComponent*>(component);

		component = entity->GetComponent(TRANSFORM);

		if (component == nullptr)
			continue;

		TransformComponent* transform = static_cast<TransformComponent*>(component);

		if (transform->TransformEnabled == false)
			continue;

		if ((transform->Position.x == static_cast<int>(ui->PreviousPosition.x)) && (transform->Position.y == static_cast<int>(ui->PreviousPosition.y))
			&& (ui->BitmapSize.x == ui->PreviousBitmapSize.x) && (ui->BitmapSize.y == ui->PreviousBitmapSize.y))
		{
			continue;
		}

		ui->PreviousPosition = XMFLOAT2(transform->Position.x, transform->Position.y);
		ui->PreviousBitmapSize = ui->BitmapSize;

		float left = ((_screenSize.Width / 2) * -1) + (transform->Position.x);
		float right = left + ui->BitmapSize.x;

		float top = (_screenSize.Height / 2) - transform->Position.y;
		float bottom = top - ui->BitmapSize.y;

		float zBuffer = transform->Position.z;

		Vertex* vertices = new Vertex[appearance->Model.VertexCount];
		if (!vertices) throw Exception("Failed to initialise vertices for bitmap");

		vertices[0].position = XMFLOAT3(left, top, zBuffer);
		vertices[0].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[1].position = XMFLOAT3(right, bottom, zBuffer);
		vertices[1].texture = XMFLOAT2(1.0f, 1.0f);

		vertices[2].position = XMFLOAT3(left, bottom, zBuffer);
		vertices[2].texture = XMFLOAT2(0.0f, 1.0f);

		vertices[3].position = XMFLOAT3(left, top, zBuffer);
		vertices[3].texture = XMFLOAT2(0.0f, 0.0f);

		vertices[4].position = XMFLOAT3(right, top, zBuffer);
		vertices[4].texture = XMFLOAT2(1.0f, 0.0f);

		vertices[5].position = XMFLOAT3(right, bottom, zBuffer);
		vertices[5].texture = XMFLOAT2(1.0f, 1.0f);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		HRESULT result = _direct3D->GetDeviceContext()->Map(appearance->Model.VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		if (FAILED(result)) throw Exception("Failed to map vertex buffer to the Device Context");

		Vertex* verticesPtr = static_cast<Vertex*>(mappedResource.pData);

		memcpy(verticesPtr, static_cast<void*>(vertices), (sizeof(Vertex) * appearance->Model.VertexCount));

		_direct3D->GetDeviceContext()->Unmap(appearance->Model.VertexBuffer, 0);

		delete[] vertices;
		vertices = nullptr;
	}
}

void UISystem::Render(vector<Entity*>& entities)
{
}