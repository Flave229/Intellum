#pragma once
#include "ISystem.h"
#include "../../Input/Input.h"

class ButtonSystem : public ISystem
{
private:
	Input* _input;

public:
	ButtonSystem(Input* input);
	~ButtonSystem() override = default;
	void Shutdown() override;

	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
};
