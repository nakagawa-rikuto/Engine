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
	float speed_ = 0.1f;           // 通常移動速度
	float boostMultiplier_ = 10.0f; // ブースト時の移動倍率
	float boostDirection_ = 0.0f;  // ブースト開始時の角度
	float verticalSpeed_ = 3.0f;   // 上昇・下降速度
	float gravity_ = -9.8f;         // 重力加速度
	float verticalVelocity_ = 0.0f;// 現在のY軸速度
	float groundLevel_ = 0.0f;     // 地面のY座標（着地時の基準）
	float rotationSpeed_ = 0.05f; // 視点回転速度
	float maxPitch_ = 1.2f;       // 上下回転の制限角度
	float deltaTime_ = 1.0f / 60.0f; // フレーム時間

	bool SetFPSCamera_ = true;

private:
	// プレイヤーの移動
	void PlayerMove();
	// 角度を指定してスナップ
	float SnapAngle(float angle, float step);
	// 角度の正規化
	float NormalizeAngle(float angle);
};

