#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

// AABB
struct AABB {
	Vector3 min; //!<最小点
	Vector3 max; //!<最大点
};

// AABBとAABBの当たり判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);
// AABBとVector2の当たり判定
bool IsCollision(const AABB& aabb1, const Vector2& position);
bool IsCollisionScreen(const AABB& aabb1, const Vector2& position);

