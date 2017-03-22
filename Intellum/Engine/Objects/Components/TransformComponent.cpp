#include "TransformComponent.h"

TransformComponent::TransformComponent() : TransformComponent(XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0))
{
}

TransformComponent::TransformComponent(XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale) : IComponent("Transform"), Position(position), Rotation(rotation), Scale(scale)
{
}