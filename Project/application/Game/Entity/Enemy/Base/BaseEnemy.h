#pragma once
/// ===Include=== ///
#include "Engine/Collider/SphereCollider.h"

// c++
#include <random>

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// Enemy
///=====================================================///
class BaseEnemy : public SphereCollider {
public:
	BaseEnemy() = default;
	~BaseEnemy();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override = 0;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Quaternion GetRotate()const;

public: /// ===Setter=== ///

	void SetPlayer(Player* player);

protected: /// ===変数の宣言=== ///

	Camera* camera_ = nullptr; // カメラ

	Player* player_ = nullptr; // Player

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

	// ランダムシード
	std::mt19937 randomEngine_;
};

