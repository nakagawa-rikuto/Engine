#pragma once
#include "Math/MathData.h"

class Collider
{
public:
	
	void Initialize(Vector3 radius);

	void Update(Vector3 center);

	AABB GetAABB() { return aabb; }

private:

	Vector3 radius;

	AABB aabb;
};

