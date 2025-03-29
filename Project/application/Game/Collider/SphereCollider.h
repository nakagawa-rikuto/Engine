#pragma once
/// ===Include=== ///
#include "Collider.h"

///=====================================================/// 
/// SphereCollider
///=====================================================///
class SphereCollider : public Collider {
public:

	SphereCollider() = default;
	~SphereCollider() = default;

	// 初期化
	virtual void Initialize() override;
	// 更新
	virtual void Update() = 0;
	// 描画
	virtual void Draw() = 0;

public: /// ===衝突=== ///
	virtual void OnCollision() override = 0;

protected:
	// 半径
	float radius_;
};

