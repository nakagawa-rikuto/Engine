#pragma once
/// ===Include=== ///
// Collider
#include "application/Game/Collider/SphereCollider.h"

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// Player
///=====================================================///
class Player : public SphereCollider {
public:
	Player() = default;
	~Player() = default;

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

public: /// ===衝突=== ///
	void OnCollision(Collider* collider) override;

private: /// ===変数=== ///

	/// ===振る舞い（Behavior）=== ///
	enum class Behavior {
		kRoot,      // 通常状態
		kMove,		// 移動
		kCharge,	// 突進
	};
	// 現在の振る舞い
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	//NOTE:使うかも？
	// 一個前のBehavior
	Behavior preBehavior_ = behavior_;
	// 次のBehavior
	Behavior nextBehavior_ = behavior_;


	/// ===基本情報=== ///

	// translate
	Vector3 translate_ = Vector3(0.0f, 0.0f, 30.0f);
	// rotate
	Vector3 rotate_ = Vector3(0.0f, 0.0f, 0.0f);
	// scale
	Vector3 scale_ = Vector3(1.0f, 1.0f, 1.0f);
	// color
	Vector4 color_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	// 移動量
	Vector3 velocity_ = Vector3(0.0f, 0.0f, 0.0f);

	// 移動
	struct MoveInfo {
		float speed_ = 4.0f;
		
	};
	MoveInfo moveInfo_;
	// 突進
	struct ChargeInfo {
		float speed_;
		float activeTimer_ = 0.15f;;
		float coolTime_ = 0.5f;
		float invincibleTime_ = 0.1f;
		bool isActive_ = false;
	};
	ChargeInfo chargeInfo_;

	// 時間の速度
	const float deltaTime_ = 1.0f / 60.0f;

private: /// ===関数=== ///

	// 通常
	void InitRoot();
	void UpdateRoot();

	// 移動
	void InitMove();
	void UpdateMove();

	// 突進
	void InitCharge();
	void UpdateCharge();
};

