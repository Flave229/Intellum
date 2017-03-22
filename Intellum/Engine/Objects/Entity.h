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

	void AddComponent(IComponent* component);
	void RemoveComponent(string componentName); 
	IComponent* GetComponent(string componentName);
};
