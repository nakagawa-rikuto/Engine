#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/OBBCollider.h"
// State
#include "application/Game/Entity/Enemy/State/Base/EnemyState.h"
// c++
#include <random>

/// ===前方宣言=== ///
class Player;

/// ===StateType=== ///
enum class StateType {
	Move,
	Attack
};

///=====================================================/// 
/// Enemy
///=====================================================///
class BaseEnemy : public OBBCollider {
public:
	BaseEnemy() = default;
	~BaseEnemy();

	// 初期化
	virtual void Initialize()override;
	// 更新
	virtual void Update()override;
	// 描画
	virtual void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;
	// ImGui
	virtual void UpdateImGui();
	// 攻撃処理
	virtual void Attack() = 0;

public: /// ===衝突判定=== ///
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///
	// Transform
	Vector3 GetTranslate()const;
	Quaternion GetRotate()const;
	// Timer
	float GetAttackTimer()const;
	// Flag
	bool GetAttackFlag()const;

public: /// ===Setter=== ///

	// Transform
	void SetTranslate(const Vector3& translate);
	void SetRotate(const Quaternion& rotate);
	void SetVelocity(const Vector3& vel);
	void SetColor(const Vector4& color);
	// Timer
	void SetTimer(StateType type, float time);
	// Player
	void SetPlayer(Player* player);

public: /// ===State用関数=== ///
	// 移動処理の開始処理
	void CommonMoveInit();
	// 移動処理の共通部分
	void CommonMove();
	// 攻撃可能かチェック
	bool CheckAttackable();
	// Stateの変更
	void ChangeState(std::unique_ptr<EnemyState> newState);

protected: /// ===変数の宣言=== ///

	Camera* camera_ = nullptr; // カメラ
	Player* player_ = nullptr; // Player

	/// ===State=== ///
	std::unique_ptr<EnemyState> currentState_;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 translate = { 0.0f, 1.0f, 0.0f };
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector3 scale = { 1.0f, 1.0f, 1.0f };
		Vector4 color = { 1.0f, 0.0f, 1.0f, 1.0f };
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};
	BaseInfo baseInfo_;

	/// ===移動情報=== ///
	struct MoveInfo {
		float timer;	// タイマー
		float speed;	// 移動速度
		float range;	// 移動範囲
		float interval;	// 移動間隔
		float waitTime;	// 待機時間

		Vector3 rangeCenter; // 移動範囲の中心
		Vector3 direction;	 // 移動方向

		bool isWating;	// 待機フラグ
	};
	MoveInfo moveInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		float timer;	// タイマー
		float range;	// 攻撃範囲（回転の情報から±）
		float distance;	// 攻撃可能距離
		float interval;	// 攻撃間隔
		int32_t power;	// 待機時間

		Vector3 direction;  // 攻撃方向
		Vector3 playerPos;  // プレイヤーの位置

		bool isAttack;	// 攻撃フラグ
	};
	AttackInfo attackInfo_;

	// 時間の経過速度
	const float deltaTime_ = 1.0f / 60.0f;

	// ランダムシード
	std::mt19937 randomEngine_;

protected: /// ===関数の宣言=== ///
	// 回転更新関数
	void UpdateRotationTowards(const Vector3& direction, float lerpT);

private:
	// 方向の設定と待機時間の設定
	void PreparNextMove(const Vector3& vector);

	// 時間を進める
	void advanceTimer();
	
};

