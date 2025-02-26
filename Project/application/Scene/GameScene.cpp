#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// c++
#include <algorithm> 
// Service
#include "Engine/System/Service/Input.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();

	// model
	model_.reset();
	// camera
	FPSCamera_.reset();
	fixedPointCamera_.reset();
	// Ground
	ground_.reset();
	// SkyDome
	skyDome_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// model
	model_ = std::make_unique<Model>();
	model_->Initialize("MonsterBall");  // 初期化(const std::string& modelNameが必須)
	model_->Update();

	// FPSCamera
	FPSCamera_ = std::make_shared<Camera>();
	FPSCamera_->Initialize();
	// fixedPointCamera
	fixedPointCamera_ = std::make_shared<Camera>();
	fixedPointCamera_->Initialize();
	// カメラマネージャにカメラを追加
	cameraManager_->Add("FPSCamera", FPSCamera_);
	cameraManager_->Add("fixedPointCamera", fixedPointCamera_);
	// アクティブカメラの設定
	cameraManager_->SetActiveCamera("FPSCamera");

	// Ground
	ground_ = std::make_unique<Ground>();
	ground_->Initialize("Ground");
	// SkyDome
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize("skydome");
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");
	ImGui::End();

	ImGui::Begin("PlayerInfo");
	ImGui::DragFloat("verticalSpeed", &verticalSpeed_, 0.01f);
	ImGui::DragFloat("gravity", &gravity_, 0.01f);
	ImGui::DragFloat("rotationSpeed", &rotationSpeed_, 0.01f);
	ImGui::DragFloat("maxPitch", &maxPitch_, 0.01f);
	
	
	ImGui::End();

	ImGui::Begin("Boost");
	ImGui::Checkbox("isBoosting", &isBoosting_);
	ImGui::DragFloat("BoostDirection", &boostDirection_, 0.01f);
	ImGui::DragFloat("BoostSpeed", &boostSpeed_, 0.01f);
	ImGui::DragFloat("BoostEnergy", &boostEnergy_, 1.0f);
	ImGui::DragFloat("boostEnergyDrain", &boostEnergyDrain_, 0.01f);
	ImGui::DragFloat("boostEnergyRegen", &boostEnergyRegen_, 0.01f);
	ImGui::DragFloat("BoostCoolTime", &boostCooldown_, 0.01f);
	ImGui::DragFloat("normalSpeed", &normalSpeed_, 0.01f);
	ImGui::DragFloat("decelerationRate", &decelerationRate_, 0.01f);

	ImGui::DragFloat("MaxBoostEnergy", &maxBoostEnergy_, 1.0f);
	ImGui::DragFloat("MaxBoostCoolTime", &maxBoostCooldown_, 1.0f);
	ImGui::DragFloat("MaxBoostSpeed", &maxBoostSpeed_, 1.0f);
	ImGui::End();

#endif // USE_IMGUI

	/// ===カメラの切り替え=== ///
	if (Input::TriggerKey(DIK_SPACE)) {
		if (SetFPSCamera_) {
			cameraManager_->SetActiveCamera("fixedPointCamera");
			SetFPSCamera_ = false;
		} else {
			cameraManager_->SetActiveCamera("FPSCamera");
			SetFPSCamera_ = true;
		}
	}

	// プレイヤーの移動
	PlayerMove();

	// Ground
	ground_->Update(cameraManager_->GetActiveCamera().get());
	// SkyDome
	skyDome_->Update(cameraManager_->GetActiveCamera().get());

	// model
	model_->SetPosition(modelTranslate_);
	model_->SetRotate(modelRotate_);
	model_->SetScale(modelScale_);
	model_->SetCamera(cameraManager_->GetActiveCamera().get());
	model_->Update();

	// FPSCamera
	FPSCamera_->SetTarget(&modelTranslate_, &modelRotate_);
	FPSCamera_->SetOffset(cameraOffset_);
	FPSCamera_->SetFollowSpeed(1.0f);

	// fixedPointCamera
	fixedPointCamera_->SetTranslate({0.0f, 5.0f, -40.0f});
	// 全てのカメラの更新
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	ground_->Draw();
	//skyDome_->Draw();
	// model
	model_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

///-------------------------------------------/// 
/// プレイヤーの移動
///-------------------------------------------///
void GameScene::PlayerMove() {
	// 左スティックの入力取得（通常移動用）
	StickState leftStick = Input::GetLeftStickState(0);

	// 右スティックの入力取得（視点回転用）
	StickState rightStick = Input::GetRightStickState(0);

	// **ブーストしていない時は視点回転 & 上昇処理**
	if (!isBoosting_) {
		modelRotate_.y += rightStick.x * rotationSpeed_; // 左右回転（Yaw）
		modelRotate_.x = std::clamp(modelRotate_.x + rightStick.y * rotationSpeed_, -maxPitch_, maxPitch_); // 上下回転制限

		// **上昇処理（Aボタン）**
		if (Input::PushButton(0, ControllerButtonType::A)) {
			verticalVelocity_ = verticalSpeed_; // 上昇
		} else {
			verticalVelocity_ += gravity_ * deltaTime_; // 重力による落下
		}
	} else {
		verticalVelocity_ = 0.0f;
	}

	// **ブーストボタンの入力**
	bool boostButtonPressed = Input::PushButton(0, ControllerButtonType::RT) || Input::PushButton(0, ControllerButtonType::LT);

	// **ブースト管理**
	if (boostButtonPressed && !isBoosting_ && boostEnergy_ > 0.0f) {
		isBoosting_ = true;
		boostDirection_ = modelRotate_.y; // 現在の向きを記録
		boostSpeed_ = maxBoostSpeed_; // ブースト速度設定
	}

	// **ブースト処理**
	if (isBoosting_) {
		// エネルギー消費
		boostEnergy_ -= boostEnergyDrain_ * deltaTime_;

		// ブーストボタンを離した or エネルギー切れ → ブースト終了
		if (!boostButtonPressed || boostEnergy_ <= 0.0f) {
			isBoosting_ = false;
			boostCooldown_ = maxBoostCooldown_; // クールタイム設定
		}
	}

	// **減速処理（ブースト終了後は通常速度まで減速）**
	if (!isBoosting_ && boostSpeed_ > normalSpeed_) {
		boostSpeed_ -= decelerationRate_ * deltaTime_;
	} else if (!isBoosting_ && boostSpeed_ < normalSpeed_) {
		boostSpeed_ = normalSpeed_; // 通常速度に戻す
	}

	// **クールタイム処理**
	if (boostCooldown_ > 0.0f) {
		boostCooldown_ -= deltaTime_;
	}

	// **ブースト時は向いている方向に固定して移動**
	float adjustedYaw = isBoosting_ ? boostDirection_ : modelRotate_.y;
	Vector3 forward(std::sin(adjustedYaw), 0.0f, std::cos(adjustedYaw));
	Vector3 right(forward.z, 0.0f, -forward.x);

	// **移動ベクトルを計算**
	Vector3 moveDirection = isBoosting_ ? forward : (forward * leftStick.y + right * leftStick.x);

	// **移動入力があるかチェック（ブースト時は処理しない）**
	if (!isBoosting_ && (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f)) {
		Normalize(moveDirection); // 正規化して一定の速度で移動
	}

	// **地面との衝突処理**
	if (modelTranslate_.y + verticalVelocity_ * deltaTime_ < groundLevel_) {
		modelTranslate_.y = groundLevel_;
		verticalVelocity_ = 0.0f;
	} else {
		modelTranslate_.y += verticalVelocity_ * deltaTime_;
	}

	// **プレイヤーの位置を更新**
	modelTranslate_ += moveDirection * boostSpeed_ * deltaTime_;

	// **エネルギー回復（ブーストが終わっている時のみ）**
	if (!isBoosting_ && boostEnergy_ < maxBoostEnergy_) {
		boostEnergy_ += boostEnergyRegen_ * deltaTime_;
	}

	// **カメラオフセットの補間**
	float targetBlend = isBoosting_ ? 1.0f : 0.0f;
	boostBlend_ = Lerp(boostBlend_, targetBlend, cameraLerpSpeed_ * deltaTime_);
	cameraOffset_ = Lerp(normalOffset_, boostOffset_, boostBlend_);
}

///-------------------------------------------/// 
/// 角度を指定したステップにスナップ（45度ごと）
///-------------------------------------------///
float GameScene::SnapAngle(float angle, float step) {
	return std::round(angle / step) * step;
}

///-------------------------------------------/// 
/// 角度の正規化（-π ~ π にする）
///-------------------------------------------///
float GameScene::NormalizeAngle(float angle) {
	while (angle > Pi()) angle -= 2.0f * Pi();
	while (angle < -Pi()) angle += 2.0f * Pi();
	return angle;
}