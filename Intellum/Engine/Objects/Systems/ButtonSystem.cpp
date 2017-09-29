#include "ButtonSystem.h"
#include "../Components/AppearanceComponent.h"
#include "../Components/UIComponent.h"
#include "../Components/ButtonComponent.h"
#include "../Components/TransformComponent.h"
#include "../Components/CollisionComponent.h"

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

		for (Entity* collidingEntity : entities)
		{
			component = collidingEntity->GetComponent(COLLISION);
			if (component == nullptr) continue;
			CollisionComponent* collidingEntityCollision = static_cast<CollisionComponent*>(component);

			if (collidingEntityCollision->CollisionType != CURSOR)
				continue;

			component = collidingEntity->GetComponent(TRANSFORM);
			if (component == nullptr) continue;
			XMFLOAT3 cursorPosition = static_cast<TransformComponent*>(component)->Position;

			component = collidingEntity->GetComponent(APPEARANCE);
			if (component == nullptr) continue;
			XMFLOAT3 cursorSize = static_cast<AppearanceComponent*>(component)->Model.Size;

			if (cursorPosition.x + cursorSize.x > transform->Position.x
				&& cursorPosition.x - cursorSize.x < transform->Position.x + ui->BitmapSize.x
				&& cursorPosition.y + cursorSize.y > transform->Position.y
				&& cursorPosition.y - cursorSize.y < transform->Position.y + ui->BitmapSize.y)
			{
				appearance->Color.Color = XMFLOAT4(0.35f, 0.35f, 0.35f, 1.0f);

				if (_input->IsControlPressed(LEFT_CLICK))
					button->OnClickCommand->Execute();
			}
			else
				appearance->Color.Color = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		}
	}
}

void ButtonSystem::Render(vector<Entity*>& entities)
{
}