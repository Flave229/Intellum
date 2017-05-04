#pragma once
#include "ISystem.h"
#include "../../Input/Input.h"

class ButtonSystem : public ISystem, public IObserver
{
private:
	XMFLOAT2 _mousePosition;

public:
	ButtonSystem();
	~ButtonSystem() override = default;
	void Shutdown() override;

	void Update(vector<Entity*>& entities, float delta) override;
	void Render(vector<Entity*>& entities) override;
	void Notify(ObserverEvent observerEvent) override;
};
