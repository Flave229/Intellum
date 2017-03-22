#include "Entity.h"
#include <d3d11.h>

Entity::Entity()
{
	Initialise();
}

Entity::~Entity()
{
}

void Entity::Initialise()
{
	GUID id;
	_id = CoCreateGuid(&id);
}

void Entity::AddComponent(IComponent* component)
{
	_components.push_back(component);
}

void Entity::RemoveComponent(string componentName)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->Name != componentName)
			continue;

		_components.erase(_components.begin() + i);
		return;
	}
}

IComponent* Entity::GetComponent(string componentName)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->Name != componentName)
			continue;

		return _components.at(i);
	}

	return nullptr;
}