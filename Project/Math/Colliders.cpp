#include "Colliders.h"

#include "Engine/Core/WinApp.h"
#include "MatrixMath.h"

///-------------------------------------------/// 
/// AABBとAABB
///-------------------------------------------///
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	// x軸方向の判定
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x) {
		return false;
	}

	// y軸方向の判定
	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y) {
		return false;
	}

	// z軸方向の判定
	if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z) {
		return false;
	}

	// 全ての軸で重なっている場合は衝突とみなす
	return true;
}

///-------------------------------------------/// 
/// AABBとVector2(World座標系)
///-------------------------------------------///
bool IsCollision(const AABB& aabb1, const Vector2& position) {
	// X軸方向の判定
	if (position.x < aabb1.min.x || position.x > aabb1.max.x) {
		return false;
	}

	// Y軸方向の判定
	if (position.y < aabb1.min.y || position.y > aabb1.max.y) {
		return false;
	}

	// Z軸方向は無視

	// 全ての条件を満たす場合は衝突している
	return true;
}

///-------------------------------------------/// 
/// AABBとVector2(Screen座標系)
///-------------------------------------------///
bool IsCollisionScreen(const AABB& aabb1, const Vector2& position) {
	// Screen座標からWorld座標への変換
	Vector3 worldPosition = TransformCoordinates(
		Vector3(position.x, position.y, 1.0f),
		Inverse4x4(MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f))
	);

	// 2DとしてZ成分を無視してAABBとの判定
	return IsCollision(aabb1, Vector2(worldPosition.x, worldPosition.y));
}
