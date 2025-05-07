#pragma once
/// ===Include=== ///
#include "application/Game/Collider/AABBCollider.h"

///=====================================================/// 
/// Ground
///=====================================================///
class Ground : public AABBCollider {
public:
	Ground() = default;
	~Ground();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw()override;

public:
	void OnCollision(Collider* collider)override;
};

