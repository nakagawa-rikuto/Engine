#include "Collider.h"

void Collider::Initialize(Vector3 radius)
{
	this->radius = radius;
}

void Collider::Update(Vector3 center)
{
	aabb.min = center - radius;
	aabb.max = center + radius;
}
