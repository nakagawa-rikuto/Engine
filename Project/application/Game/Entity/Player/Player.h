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

	// 初期化（GameScene用）
	void Init(Camera* camera);

	// 初期化
	void Initialize()override;
	// 更新
	void Update()override;
	// 描画
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;
	// ImGui
	void UpdateImGui();

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	Vector3 GetTranslate()const;
	Quaternion GetRotate()const;

private: /// ===変数の宣言=== ///

	Camera* camera_ = nullptr; // カメラ

	/// ===振る舞い=== ///
	enum class Behavior {
		kRoot,
		kMove,
		kAvoidance,
		kCharge,
		kAttack
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

	/// ===無敵時間の情報=== ///
	struct InvicibleInfo {
		float time = 1.0f;   // 無敵時間
		float timer = 0.0f;  // 無敵タイマー
		bool isFlag = false; // 無敵フラグ
	};
	InvicibleInfo invicibleInfo_;

	/// ===移動情報=== ///
	struct MoveInfo {
		float speed = 0.5f;               // 移動速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	MoveInfo moveInfo_;

	/// ===突進情報=== ///
	struct ChargeInfo {
		float speed = 0.0f;			  // 突進時の移動速度
		float activeTime = 0.15f;      // 突進の有効時間
		float cooltime = 0.5f;		  // 突進のクールタイム
		float invincibleTime = 0.1f;  // 突進時の無敵時間
		float timer = 0.0f;           // 突進のタイマー
		float acceleration = 0.0f;    // 突進の加速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
		bool isFlag = false;		  // 突進のフラグ
	};
	ChargeInfo chargeInfo_;

	/// ===回避情報=== ///
	struct AvoidanceInfo {
		float speed = 0.0f;			  // 回避時の移動速度
		float activeTime = 0.3f;      // 回避の有効時間
		float cooltime = 0.8f;		  // 回避のクールタイム
		float invincibleTime = 0.3f;  // 回避時の無敵時間
		float timer = 0.0f;           // 回避のタイマー
		float acceleration = 0.0f;    // 回避の加速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
		bool isFlag = false;		  // 回避のフラグ
	};
	AvoidanceInfo avoidanceInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		// 攻撃目標のリスト(Enemy : マーク用)
		// 攻撃目標のリスト(Enemy : 攻撃用)
		float reachTime = 0.1f;               // 攻撃の最終座標までの到達時間
		float ratio = 0.0f;                   // 攻撃の移動割合
		const float freezeTime = 0.3f;		  // 攻撃完了から爆破までの時間
		float freezeTimer = 0.0f;					  // 
		Vector3 startPos = {0.0f, 0.0f, 0.0f};					  // 攻撃の最初の座標
		Vector3 endPos = { 0.0f, 0.0f, 0.0f };						  // 攻撃の最終の座標
		bool isFlag = false;				  // 攻撃のフラグ
		bool isSetTarget = false;			  // ターゲットのフラグ
	};
	AttackInfo attackInfo_;

	// 時間の経過速度
	const float deltaTime_ = 1.0f / 60.0f;
private:
	// Root
	void InitRoot();
	void UpdateRoot();
	// Move
	void InitMove();
	void UpdateMove();
	// Avoidance
	void InitAvoidance();
	void UpdateAvoidance();
	// Charge
	void InitCharge();
	void UpdateCharge();
	// Attack
	void InitAttack();
	void UpdateAttack();

	// 時間を進める
	void advanceTimer();
};

