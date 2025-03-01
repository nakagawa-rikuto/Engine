#pragma once
/// ===Include=== ///
// c++
#include <optional>
// application
#include "application/Drawing/3d/Model.h"

/// ===前方宣言=== ///
class Camera;

///-------------------------------------------/// 
/// プレイヤー
///-------------------------------------------///
class Player {
public:

	Player() = default;
	~Player();

	void Initialize(const std::string& modelName);
	void Update(Camera* camera);
	void Draw();

	void ImGuiUpdate();

private:
	// 振る舞い（Behavior）
	enum class Behavior {
		kRoot, // 通常状態
		kMove, // 移動
		kBoost // ブースト
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	// モデル
	std::unique_ptr<Model> model_;

	/// ===変数=== ///
	Vector3 translate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 rotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// Root
	

	// Move
	float normalSpeed_ = 3.0f;        // 通常時の移動速度

	// Boost
	bool isBoosting_ = false;  // ブースト管理フラグ
	float boostDirection_ = 0.0f;     // ブースト時の向き
	float boostSpeed_ = 0.0f;         // 現在の速度
	float boostEnergy_ = 10000.0f;      // 現在のエネルギー
	float maxBoostEnergy_ = 100.0f;   // 最大エネルギー
	float boostEnergyDrain_ = 20.0f;  // 毎秒エネルギー消費量
	float boostEnergyRegen_ = 10.0f;  // 毎秒エネルギー回復量
	float boostCooldown_ = 0.0f;      // ブーストのクールタイム
	float maxBoostCooldown_ = 3.0f;   // クールタイム最大値
	float maxBoostSpeed_ = 20.0f;     // ブースト時の最高速度
	float decelerationRate_ = 10.0f;  // ブースト終了後の減速率
	float boostTurnSpeed_ = 1.0f; // ブースト中の旋回速度

	// Jump
	bool isJump_ = false;
	float verticalSpeed_ = 3.0f;   // 上昇・下降速度
	float gravity_ = -9.8f;         // 重力加速度
	float verticalVelocity_ = 0.0f;// 現在のY軸速度
	float groundLevel_ = 0.0f;     // 地面のY座標（着地時の基準）

	// Camera
	Vector3 cameraOffset_ = { 0.0f, 0.0f, 0.0f }; // 現在のオフセット
	Vector3 normalOffset_ = { 0.0f, 0.5f, -10.0f }; // 通常のオフセット
	Vector3 boostOffset_ = { 2.0f, 0.8f, -10.0f }; // ブースト時のオフセット
	Vector3 cameraTranslate_ = { 0.0f, 0.0f, 0.0f }; // カメラの位置
	Vector3 cameraRotate_ = { 0.0f, 0.0f, 0.0f }; // カメラの回転
	float rotationSpeed_ = 0.05f; // 視点回転速度
	float maxPitch_ = 1.2f;       // 上下回転の制限角度
	float deltaTime_ = 1.0f / 60.0f; // フレーム時間
	float cameraLerpSpeed_ = 4.0f; // 補間速度
	float boostBlend_ = 0.0f; // 補間係数

	// 地面との当たり判定フラグ
	bool isCollisionGround_ = true;

private:
	// 通常
	void InitializeRoot(); //初期化
	void UpdateRoot(); // 更新
	// 移動
	void InitializeMove(); //初期化
	void UpdateMove(); // 更新
	// ブースト
	void InitializeBoost(); //初期化
	void UpdateBoost(); // 更新
	// カメラ
	void UpdateCamera();
	// 上昇
	void UpdateAir();
};

