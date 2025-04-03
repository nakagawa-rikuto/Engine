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
	virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Getter=== ///
	Sphere GetSphere()const;

protected:
	Sphere sphere_;
};

