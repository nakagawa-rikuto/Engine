#pragma once
/// ===Include=== ///
#include "application/Game/Collider/Base/Collider.h"

///=====================================================/// 
/// AABB
///=====================================================///
class AABBCollider : public Collider {
public:

	AABBCollider() = default;
	~AABBCollider() = default;

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;

public: /// ===衝突=== ///
	virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Getter=== ///
	AABB GetAABB() const;

protected:
	AABB aabb_;
};

