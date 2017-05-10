#include "InputSystem.h"
#include "../Components/InputComponent.h"
#include "../../Input/ControlCommand.h"

InputSystem::InputSystem(Input* input) : _input(input)
{
}

void InputSystem::Shutdown()
{
}

void InputSystem::Update(vector<Entity*>& entities, float delta)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(INPUT_COMPONENT);
		if (component == nullptr) continue;
		InputComponent* input = static_cast<InputComponent*>(component);

		for (ControlCommand& controlCommand : input->ControlCommands)
		{
			if (controlCommand.CurrentCooldown < controlCommand.Cooldown)
			{
				controlCommand.CurrentCooldown += delta;
				continue;
			}

			if (_input->IsControlPressed(controlCommand.Control))
			{
				controlCommand.Command->Execute();
				controlCommand.CurrentCooldown = 0;
			}
		}
	}
}

void InputSystem::Render(vector<Entity*>& entities)
{
}