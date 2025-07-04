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

private: /// ===変数=== ///

	// 座標
	Vector3 translate = { 0.0f, 0.0f, 0.0f }; 

	// 回転
	Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f }; 

	// 拡縮
	Vector3 scale = { 1.0f, 1.0f, 1.0f }; 

	// ベクトル
	Vector3 velocity = { 0.0f, 0.0f, 0.0f }; 

	// 速度
	float speed = 1.0f;
};

