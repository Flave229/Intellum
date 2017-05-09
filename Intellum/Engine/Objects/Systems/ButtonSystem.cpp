#include "ButtonSystem.h"
#include "../Components/AppearanceComponent.h"
#include "../Components/UIComponent.h"
#include "../Components/ButtonComponent.h"
#include "../Components/TransformComponent.h"

ButtonSystem::ButtonSystem(Input* input) : _input(input)
{
}

void ButtonSystem::Shutdown()
{
}

void ButtonSystem::Update(vector<Entity*>& entities, float delta)
{
	for(int i = 0; i < entities.size(); i++)
	{
		Entity* entity = entities.at(i);
		IComponent* component = entity->GetComponent(BUTTON);
		if (component == nullptr) continue;
		ButtonComponent* button = static_cast<ButtonComponent*>(component);
			
		component = entity->GetComponent(APPEARANCE);
		if (component == nullptr) continue;
		AppearanceComponent* appearance = static_cast<AppearanceComponent*>(component);
		if (appearance->RenderEnabled == false) continue;

		component = entity->GetComponent(USER_INTERFACE);
		if (component == nullptr) continue;
		UIComponent* ui = static_cast<UIComponent*>(component);

		component = entity->GetComponent(TRANSFORM);
		if (component == nullptr) continue;
		TransformComponent* transform = static_cast<TransformComponent*>(component);

		XMFLOAT2 mousePosition = _input->MousePosition;

		if (mousePosition.x > transform->Position.x
			&& mousePosition.x < transform->Position.x + ui->BitmapSize.x
			&& mousePosition.y > transform->Position.y
			&& mousePosition.y < transform->Position.y + ui->BitmapSize.y)
		{
			appearance->Color.Color = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

			if (_input->IsControlPressed(LEFT_CLICK))
				button->OnClickCommand->Execute();
		}
		else
			appearance->Color.Color = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
	}
}

void ButtonSystem::Render(vector<Entity*>& entities)
{
}