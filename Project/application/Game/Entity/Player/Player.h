#pragma once
/// ===Include=== ///
#include "application/Game/Collider/SphereCollider.h"

///=====================================================/// 
/// Player
///=====================================================///
class Player : public SphereCollider {
public:

	Player() = default;
	~Player();

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw()override;
	// ImGui
	void UpdateImGui();

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Vector3 GetRotate()const;

private: /// ===変数の宣言=== ///

	/// ===振る舞い=== ///
	enum class Behavior {
		kRoot,
		kMove,
		kCharge,
		kAttack
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Vector3 rotate = { 0.0f, 0.0f, 0.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

	/// ===移動情報=== ///
	struct MoveInfo {
		float speed;               // 移動速度
		float invincibleTime;      // 無敵時間
		float invincibleTimer;     // 無敵タイマー
		bool isInvincible = false; // 無敵フラグ
	};
	MoveInfo moveInfo_;

	/// ===突進情報=== ///
	struct ChargeInfo {
		float speed;          // 突進時の移動速度
		float activeTime;     // 突進の有効時間
		float cooltime;		  // 突進のクールタイム
		float InvincibleTime; // 突進時の無敵時間
		float timer;          // 突進のタイマー
		bool isFlag = false;  // 突進のフラグ
	};
	ChargeInfo chargeInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		// 攻撃目標のリスト(Enemy : マーク用)
		// 攻撃目標のリスト(Enemy : 攻撃用)
		float reachTime;               // 攻撃の最終座標までの到達時間
		float ratio;                   // 攻撃の移動割合
		const float freezeTime = 0.3f; // 攻撃完了から爆破までの時間
		float freezeTimer;             // 
		Vector3 startPos;              // 攻撃の最初の座標
		Vector3 endPos;                // 攻撃の最終の座標
		bool isFlag = false;           // 攻撃のフラグ
		bool isSetTarget = false;      // ターゲットのフラグ
	};
	AttackInfo attackInfo_;

private:
	// Root
	void InitRoot();
	void UpdateRoot();
	// Move
	void InitMove();
	void UpdateMove();
	// Charge
	void InitCharge();
	void UpdateCharge();
	// Attack
	void InitAttack();
	void UpdateAttack();
};

