#pragma once
/// ===Include=== ///
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// 遠距離攻撃Enemy
///=====================================================///
class LongRangeEnemy : public BaseEnemy {
public:
	LongRangeEnemy() = default;
	~LongRangeEnemy();

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

private: /// ===変数=== ///

	/// ===Behavior=== ///
	enum Behavior {
		kMove, // 移動
		kAttack // 攻撃
	};
	Behavior behavior_ = Behavior::kMove; // 振る舞い
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// 移動情報
	struct MoveInfo {
		float speed = 0.05f; // 移動速度
		float range = 20.0f; // 移動範囲
		Vector3 rangeCenter = { 0.0f, 0.0f, 0.0f }; // 移動範囲の中心
		float interval = 8.0f; // 移動間隔
		float timer = 0.0f; // タイマー
	};
	MoveInfo moveInfo_; // 移動情報

	// 攻撃情報
	struct AttackInfo {
		float range = 2.0f; // 攻撃範囲(回転の情報から±)
		float distance = 15.0f; // 攻撃可能距離
		float interval = 8.0f; // 攻撃間隔
		float timer = 5.0f; // タイマー
		float power = 1.0f; // 攻撃力
		Vector3 direction = { 0.0f, 0.0f, 0.0f }; // 攻撃方向
		Vector3 playerPos = { 0.0f, 0.0f, 0.0f }; // プレイヤー位置
		bool isAttack = false; // 攻撃中かどうか
	};
	AttackInfo attackInfo_; // 攻撃情報

	// 時間の経過速度
	const float deltaTime_ = 1.0f / 60.0f;

private:
	// 移動処理
	void InitMove();
	void Move();

	// 攻撃処理
	void InitAttack();
	void Attack();
	// 攻撃可能かチェック
	bool CheckAttackable();

	// 回転更新関数
	void UpdateRotationTowards(const Vector3& direction, float lerpT);

	// 時間を進める
	void advanceTimer();
};
