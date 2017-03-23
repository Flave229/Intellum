#pragma once
#include <vector>
#include "Components/IComponent.h"

using namespace std;

class Entity
{
private:
	int _id;
	vector<IComponent*> _components;

private:
	void Initialise();

public:
	Entity();
	~Entity();

	void Shutdown();

	void AddComponent(IComponent* component);
	void RemoveComponent(ComponentType component);
	IComponent* GetComponent(ComponentType component);
};