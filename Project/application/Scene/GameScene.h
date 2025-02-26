#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"
// Game
#include "application/Drawing/3d/Model.h"
#include "application/Drawing/3d/AnimationModel.h"
#include "application/Drawing/3d/Camera.h"
#include "application/Game/Field/Ground.h"
#include "application/Game/Field/SkyDome.h"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class GameScene : public IScene {
public:/// ===メンバ関数=== ///

	GameScene() = default;
	~GameScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:/// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>
	 
	/// ===クラス=== ///
	std::unique_ptr<Model> model_;
	std::shared_ptr<Camera> FPSCamera_; // FPSカメラ
	std::shared_ptr<Camera> fixedPointCamera_; // 定点カメラ

	std::unique_ptr<Ground> ground_;
	std::unique_ptr<SkyDome> skyDome_;

	/// ===変数=== ///
	Vector3 modelTranslate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelRotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelScale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 modelColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	float verticalSpeed_ = 3.0f;   // 上昇・下降速度
	float gravity_ = -9.8f;         // 重力加速度
	float verticalVelocity_ = 0.0f;// 現在のY軸速度
	float groundLevel_ = 0.0f;     // 地面のY座標（着地時の基準）
	float rotationSpeed_ = 0.05f; // 視点回転速度
	float maxPitch_ = 1.2f;       // 上下回転の制限角度
	float deltaTime_ = 1.0f / 60.0f; // フレーム時間

	bool isBoosting_ = false;  // ブースト管理フラグ
	float boostDirection_ = 0.0f;     // ブースト時の向き
	float boostSpeed_ = 0.0f;         // 現在の速度
	float boostEnergy_ = 100.0f;      // 現在のエネルギー
	float maxBoostEnergy_ = 100.0f;   // 最大エネルギー
	float boostEnergyDrain_ = 20.0f;  // 毎秒エネルギー消費量
	float boostEnergyRegen_ = 10.0f;  // 毎秒エネルギー回復量
	float boostCooldown_ = 0.0f;      // ブーストのクールタイム
	float maxBoostCooldown_ = 3.0f;   // クールタイム最大値
	float normalSpeed_ = 3.0f;        // 通常時の移動速度
	float maxBoostSpeed_ = 20.0f;     // ブースト時の最高速度
	float decelerationRate_ = 10.0f;  // ブースト終了後の減速率

	Vector3 cameraOffset_ = { 0.0f, 0.0f, 0.0f }; // 現在のオフセット
	Vector3 normalOffset_ = { 0.0f, 0.3f, -7.0f }; // 通常のオフセット
	Vector3 boostOffset_ = { 2.0f, 0.3f, -8.0f }; // ブースト時のオフセット
	float cameraLerpSpeed_ = 4.0f; // 補間速度
	float boostBlend_ = 0.0f; // 補間係数

	bool SetFPSCamera_ = true;

private:
	// プレイヤーの移動
	void PlayerMove();
	// 角度を指定してスナップ
	float SnapAngle(float angle, float step);
	// 角度の正規化
	float NormalizeAngle(float angle);
};

