#pragma once
#include "ICommand.h"
#include "../Components/TransformComponent.h"

class ToggleTransformCommand : public ICommand
{
	TransformComponent* _transform;

public:
	ToggleTransformCommand(TransformComponent* transform);
	~ToggleTransformCommand() override = default;
	void Shutdown() override;
	void Execute() override;
};
