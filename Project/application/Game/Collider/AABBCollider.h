#pragma once
/// ===Include=== ///
#include "Collider.h"

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
	virtual void OnCollision() override = 0;

protected:
	// サイズ
	// 幅・高さ・奥行
	Vector3 size_ = { 0.0f, 0.0f, 0.0f };
};

