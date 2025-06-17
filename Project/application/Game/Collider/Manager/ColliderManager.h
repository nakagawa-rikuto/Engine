#pragma once
/// ===Include=== ///
// c++
#include <list>
// Collider
#include "application/Game/Collider/Base/Collider.h"

/// ===前方宣言=== ///
class SphereCollider;
class AABBCollider;
class OBBCollider;

///=====================================================/// 
/// ColliderManager
///=====================================================///
class ColliderManager {
public:

	ColliderManager() = default;
	~ColliderManager();

	// リセット
	void Reset();
	// 追加
	void AddCollider(Collider* collider);
	// 二つのColliderの当たり判定
	void CheckPairCollision(Collider* colliderA, Collider* colliderB);
	// 全てのColliderの当たり判定
	void CheckAllCollisions();

public: /// ===Setter=== ///
	// カメラの設定
	void SetCamera(Camera* camera);

	// Lightの設定
	void SetLight(LightType type);
	void SetLightData(LightInfo light);

private: /// ===変数=== ///

	// コライダーのリスト
	std::list<Collider*> colliders_;

private: /// ===関数=== ///

	// 球と球の当たり判定
	bool SphereToSphereCollision(class SphereCollider* a, class SphereCollider* b);
	// AABBとAABBの当たり判定
	bool AABBToAABBCollision(class AABBCollider* a, class AABBCollider* b);
	// OBBとOBBの当たり判定
	bool OBBToOBBCollision(class OBBCollider* a, class OBBCollider* b);

	// 球とAABBの当たり判定
	bool SphereToAABBCollision(class SphereCollider* sphere, class AABBCollider* aabb);
	// AABBとOBBの当たり判定
	bool AABBToOBBCollsision(class AABBCollider* aabb, class OBBCollider* obb);
	// 球とOBBの当たり判定
	bool SphereToOBBCollision(class SphereCollider* sphere, class OBBCollider* obb);

	// SATによる OBB vs OBB 判定
	bool OBBSATCollision(const OBB& a, const OBB& b);
	void ProjectOBBOntoAxis(const OBB& obb, const Vector3& axis, float& outMin, float& outMax);
};

