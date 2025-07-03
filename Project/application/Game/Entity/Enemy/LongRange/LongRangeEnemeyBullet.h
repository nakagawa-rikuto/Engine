#pragma once
/// ===Inclde=== ///
#include "Engine/Collider/SphereCollider.h"

///=====================================================/// 
/// 遠距離攻撃Enemyの弾
///=====================================================///
class LongRangeEnemeyBullet : public SphereCollider{
public:

	LongRangeEnemeyBullet() = default;
	~LongRangeEnemeyBullet();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Quaternion GetRotate()const;
};

