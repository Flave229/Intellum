#pragma once
#include "ISystem.h"
#include "../../Input/Input.h"

class InputSystem : public ISystem
{
private:
	Input* _input;

public:
	InputSystem(Input* input);
	~InputSystem() override = default;
	void Shutdown() override;

	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
};