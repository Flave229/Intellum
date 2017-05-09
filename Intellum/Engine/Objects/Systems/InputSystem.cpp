#include "InputSystem.h"
#include "../Components/InputComponent.h"

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
		
		map<Controls, ICommand*>::iterator iterator;
		for (iterator = input->ControlCommands.begin(); iterator != input->ControlCommands.end(); ++iterator)
		{
			if (_input->IsControlPressed(iterator->first))
				iterator->second->Execute();
		}
	}
}

void InputSystem::Render(vector<Entity*>& entities)
{
}