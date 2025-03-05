#include "Player.h"
// c++
#include <algorithm> 
// application
#include "application/Drawing/3d/Camera.h"
// Service
#include "Engine/System/Service/Input.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI



///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {
	model_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize(const std::string& modelName, Camera* camera) {
	// model
	model_ = std::make_unique<Model>();
	model_->Initialize(modelName);  // 初期化(const std::string& modelNameが必須)
	model_->Update();

	// Cameraの設定
	camera_ = camera;
	camera_->SetFollowCamera(FollowCameraType::Orbiting);
	InitializeRoot();
	camera_->SetFollowSpeed(1.0f);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update() {

	/// ===Behavior遷移の実装=== ///
	if (behaviorRequest_) {
		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();

		// 各振る舞いの初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:
			camera_->SetFollowCamera(FollowCameraType::Orbiting);
			InitializeRoot();
			camera_->SetFollowSpeed(1.0f);
			break;
		case Player::Behavior::kMove:
			camera_->SetFollowCamera(FollowCameraType::FixedOffset);
			InitializeMove();
			camera_->SetFollowSpeed(0.8f);
			break;
		case Player::Behavior::kBoost:
			camera_->SetFollowCamera(FollowCameraType::Interpolated);
			InitializeBoost();
			camera_->SetFollowSpeed(1.0f);
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いの更新
	switch (behavior_) {
	case Player::Behavior::kRoot:
		UpdateRoot();
		UpdateAir();
		camera_->SetRotate(cameraRotate_);
		break;
	case Player::Behavior::kMove:
		UpdateMove();
		UpdateAir();
		break;
	case Player::Behavior::kBoost:
		UpdateBoost();
		break;
	}

	UpdateCamera();

	// Cameraの更新
	camera_->SetTarget(&translate_, &rotate_);
	camera_->SetOffset(cameraOffset_);

	// モデルの更新
	model_->SetPosition(translate_);
	model_->SetRotate(rotate_);
	model_->SetScale(scale_);
	model_->SetCamera(camera_);
	model_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Player::Draw() {
	model_->Draw();
}

///-------------------------------------------/// 
/// 更新(ImGUi)
///-------------------------------------------///
void Player::ImGuiUpdate() {
	ImGui::Begin("PlayerInfo");
	ImGui::Checkbox("isCollisionGround", &isCollisionGround_);
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
}

///-------------------------------------------/// 
/// 初期化(Behavior)
///-------------------------------------------///
// 通常
void Player::InitializeRoot() {
	cameraRotate_ = rotate_;
}
// 移動
void Player::InitializeMove() {}
// ブースト
void Player::InitializeBoost() {
	isBoosting_ = true;
	boostDirection_ = rotate_.y;
	boostSpeed_ = maxBoostSpeed_;
}

///-------------------------------------------/// 
/// 更新(Behavior)
///-------------------------------------------///
// 通常
void Player::UpdateRoot() {
	/// === 左スティックの入力取得（移動用） === ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// === 右スティックの入力取得（視点回転用） === ///
	StickState rightStick = Input::GetRightStickState(0);

	/// === カメラ回転 === ///
	cameraRotate_.y += rightStick.x * rotationSpeed_;
	cameraRotate_.x = std::clamp(cameraRotate_.x + rightStick.y * rotationSpeed_, -maxPitch_, maxPitch_);

	/// === 左スティック押し込みでブースト開始 === ///
	if (Input::PushButton(0, ControllerButtonType::LeftStick)) {
		behaviorRequest_ = Behavior::kBoost;
	}

	/// === 移動があれば移動状態へ === ///
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		behaviorRequest_ = Behavior::kMove;
	}
}
// 移動
void Player::UpdateMove() {
	/// === 左スティックの入力取得 === ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// === 右スティックで進行方向調整 === ///
	StickState rightStick = Input::GetRightStickState(0);
	rotate_.y += rightStick.x * rotationSpeed_;
	cameraRotate_.x = std::clamp(rotate_.x + rightStick.y * rotationSpeed_, -maxPitch_, maxPitch_);

	/// === 移動方向の計算 === ///
	Vector3 forward(std::sin(rotate_.y), 0.0f, std::cos(rotate_.y));
	Vector3 right(forward.z, 0.0f, -forward.x);
	Vector3 moveDirection = forward * leftStick.y + right * leftStick.x;

	/// === 移動入力があるなら正規化 === ///
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		Normalize(moveDirection);
	}

	/// === エネルギー未満なら少し加速 === ///
	float speed = (boostEnergy_ < maxBoostEnergy_) ? normalSpeed_ * 1.2f : normalSpeed_;

	translate_ += moveDirection * speed * deltaTime_;

	/// === 左スティック押し込みでブースト開始 === ///
	if (Input::PushButton(0, ControllerButtonType::LeftStick)) {
		behaviorRequest_ = Behavior::kBoost;
		return;
	}

	/// === 左スティックの入力があるかチェック（しきい値 0.1f 以下なら `kRoot` に戻る） === ///
	if (std::abs(leftStick.x) < 0.1f && std::abs(leftStick.y) < 0.1f) {
		behaviorRequest_ = Behavior::kRoot;
		return;
	}
}
// ブースト
void Player::UpdateBoost() {
	/// === ブースト中の方向変更 === ///
	StickState rightStick = Input::GetRightStickState(0);
	rotate_.y += rightStick.x * rotationSpeed_;
	rotate_.x = std::clamp(rotate_.x + -rightStick.y * rotationSpeed_, -maxPitch_, maxPitch_);

	/// ===左スティックの取得=== ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// === エネルギー消費 === ///
	boostEnergy_ -= boostEnergyDrain_ * deltaTime_;

	/// === ブースト解除条件 === ///
	if (leftStick.y < -0.5f || Input::PushButton(0, ControllerButtonType::A) || boostEnergy_ <= 0.0f) {
		behaviorRequest_ = Behavior::kRoot;
	}

	/// === `rotate_` に基づいて移動ベクトルを計算 (X, Y 軸を考慮) === ///
	Vector3 forward;
	forward.x = std::cos(rotate_.x) * std::sin(rotate_.y); // X軸の傾きも考慮
	forward.y = std::sin(-rotate_.x);                      // 上下移動も考慮
	forward.z = std::cos(rotate_.x) * std::cos(rotate_.y); // Z軸

	Normalize(forward); // 速度を一定にするため正規化

	/// === ブースト移動（`rotate_` に基づく方向に進む） === ///
	translate_ += forward * boostSpeed_ * deltaTime_;
}
// カメラ
void Player::UpdateCamera() {
	/// === カメラオフセットの補間 === ///
	float targetBlend = (behavior_ == Behavior::kBoost) ? 1.0f : 0.0f;
	boostBlend_ = Lerp(boostBlend_, targetBlend, cameraLerpSpeed_ * deltaTime_);

	/// === オフセットの適用 === ///
	cameraOffset_ = Lerp(normalOffset_, boostOffset_, boostBlend_);
}
// ジャンプ
void Player::UpdateAir() {
	/// === Aボタンで上昇 === ///
	if (Input::PushButton(0, ControllerButtonType::A) && boostEnergy_ > 0.0f) {
		verticalVelocity_ = verticalSpeed_;
		boostEnergy_ -= boostEnergyDrain_ * deltaTime_;
	} else {
		verticalVelocity_ += gravity_ * deltaTime_; // 自然落下
	}

	/// === 地面との衝突 === ///
	if (translate_.y + verticalVelocity_ * deltaTime_ < groundLevel_) {
		translate_.y = groundLevel_;
		verticalVelocity_ = 0.0f;
	} else {
		translate_.y += verticalVelocity_ * deltaTime_;
	}
}
