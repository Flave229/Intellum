#include "Entity.h"
#include <d3d11.h>

Entity::Entity()
{
	Initialise();
}

Entity::~Entity()
{
}

void Entity::Shutdown()
{
	for (unsigned long long i = _components.size(); i > 0; i--)
	{
		_components.back()->Shutdown();
		_components.pop_back();
	}

	_components.clear();
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

void Entity::RemoveComponent(ComponentType component)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->Name != component)
			continue;

		_components.erase(_components.begin() + i);
		return;
	}
}

IComponent* Entity::GetComponent(ComponentType component)
{
	for (int i = 0; i < _components.size(); i++)
	{
		if (_components.at(i)->Name != component)
			continue;

		return _components.at(i);
	}

	return nullptr;
}