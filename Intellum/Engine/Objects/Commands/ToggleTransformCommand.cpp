#include "ToggleTransformCommand.h"

ToggleTransformCommand::ToggleTransformCommand(TransformComponent* transform) : _transform(transform)
{
}

void ToggleTransformCommand::Shutdown()
{
}

void ToggleTransformCommand::Execute()
{
	_transform->TransformEnabled = !_transform->TransformEnabled;
}