#include "TransformSystem.h"
#include "../Components/TransformComponent.h"

TransformSystem::TransformSystem(DirectX3D* direct3D) : _direct3D(direct3D)
{
}

void TransformSystem::Shutdown()
{
}

void TransformSystem::Update(vector<Entity*> entities, float delta)
{
	for (Entity* entity : entities)
	{
		IComponent* component = entity->GetComponent(TRANSFORM);
		
		if (component != nullptr)
		{
			TransformComponent* transformComponent = static_cast<TransformComponent*>(component);
			XMMATRIX transformation = _direct3D->GetWorldMatrix();

			UpdatePosition(transformComponent->Position, transformComponent->Velocity, delta);
			UpdateRotation(transformComponent->Rotation, transformComponent->AngularVelocity, delta);

			transformation *= XMMatrixScaling(transformComponent->Scale.x, transformComponent->Scale.y, transformComponent->Scale.z);
			transformation *= XMMatrixRotationRollPitchYaw(transformComponent->Rotation.x, transformComponent->Rotation.y, transformComponent->Rotation.z);
			transformation *= XMMatrixTranslation(transformComponent->Position.x, transformComponent->Position.y, transformComponent->Position.z);

			transformComponent->Transformation = transformation;
		}
	}
}

void TransformSystem::UpdatePosition(XMFLOAT3& position, XMFLOAT3 velocity, float delta)
{
	position.x += velocity.x * delta;
	position.y += velocity.y * delta;
	position.z += velocity.z * delta;
}

void TransformSystem::UpdateRotation(XMFLOAT3& rotation, XMFLOAT3 velocity, float delta)
{
	rotation.x = CapRotationRange(rotation.x + velocity.x * delta);
	rotation.y = CapRotationRange(rotation.y + velocity.y * delta);
	rotation.z = CapRotationRange(rotation.z + velocity.z * delta);
}

float TransformSystem::CapRotationRange(float rotation)
{
	if (rotation > XM_2PI)
	{
		rotation -= XM_2PI;
	}
	else if (rotation < 0.0f)
	{
		rotation += XM_2PI;
	}

	return rotation;
}