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

	// エネルギーの設定
	energyInfo_.rest = energyInfo_.max;
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
	camera_->SetOffset(cameraInfo_.offset);

	// モデルの更新
	model_->SetPosition(translate_);
	model_->SetRotate({rotate_.x, rotate_.y, rotate_.z});
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
	ImGui::Begin("Player");
	ImGui::DragFloat3("Translate", &translate_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &rotate_.x, 0.01f);
	ImGui::End();

	ImGui::Begin("Jump");
	ImGui::DragFloat("VerticalSpeed", &jumpInfo_.verticalSpeed, 0.01f);
	ImGui::DragFloat("Gravity", &jumpInfo_.gravity, 0.01f);
	ImGui::DragFloat("VerticalVelocity", &jumpInfo_.verticalVelocity, 0.01f);
	ImGui::End();

	ImGui::Begin("Move");
	ImGui::DragFloat("Speed", &moveInfo_.speed, 0.01f);
	ImGui::DragFloat("RotationSpeed", &moveInfo_.rotationSpeed, 0.01f);
	ImGui::DragFloat("MaxPicth", &moveInfo_.maxPitch, 0.01f);
	ImGui::End();

	ImGui::Begin("Boost");
	ImGui::DragFloat("Speed", &boostInfo_.speed, 0.01f);
	ImGui::DragFloat("CoolTime", &boostInfo_.coolDown, 0.01f);
	ImGui::DragFloat("DecelerationRate", &boostInfo_.deccelerationRate, 0.01f);
	ImGui::DragFloat("MaxBoostCoolTime", &boostInfo_.maxCoolDown, 1.0f);
	ImGui::DragFloat("MaxBoostSpeed", &boostInfo_.maxSpeed, 1.0f);
	ImGui::DragFloat("RotationSpeed", &boostInfo_.rotationSpeed, 0.01f);
	ImGui::DragFloat("MaxPicth", &boostInfo_.maxPitch, 0.01f);
	ImGui::End();

	ImGui::Begin("Energy");
	ImGui::DragFloat("Energy", &energyInfo_.rest, 1.0f);
	ImGui::DragFloat("EnergyDrain", &energyInfo_.drain, 0.01f);
	ImGui::DragFloat("EnergyRegen", &energyInfo_.regen, 0.01f);
	ImGui::DragFloat("MaxBoostEnergy", &energyInfo_.max, 1.0f);
	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("Rotate", &cameraInfo_.rotate.x, 0.01f);
	ImGui::DragFloat3("MoveOffset", &cameraInfo_.moveOffset.x, 0.01f);
	ImGui::DragFloat3("BoostOffset", &cameraInfo_.boostOffset.x, 0.01f);
	ImGui::DragFloat("LerpSpeed", &cameraInfo_.lerpSpeed, 0.1f);
	ImGui::DragFloat("Blend", &cameraInfo_.blend, 0.01f);
	ImGui::End();

}

///-------------------------------------------/// 
/// 初期化(Behavior)
///-------------------------------------------///
// 通常
void Player::InitializeRoot() {
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });
}
// 移動
void Player::InitializeMove() {

}
// ブースト
void Player::InitializeBoost() {
	boostInfo_.speed = boostInfo_.maxSpeed;
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
	camera_->SetStick({ rightStick.x, rightStick.y });

	/// === 左スティック押し込みでブースト開始 === ///
	if (Input::TriggerButton(0, ControllerButtonType::LeftStick)) {
		behaviorRequest_ = Behavior::kBoost;
		// ここで現在のカメラの向いている方向にプレイヤーを向かせる処理を追加
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
	rotate_.y += rightStick.x * moveInfo_.rotationSpeed;
	cameraInfo_.rotate.x = std::clamp(rotate_.x + rightStick.y * moveInfo_.rotationSpeed, -moveInfo_.maxPitch, moveInfo_.maxPitch);

	/// === 移動方向の計算 === ///
	Vector3 forward(std::sin(rotate_.y), 0.0f, std::cos(rotate_.y));
	Vector3 right(forward.z, 0.0f, -forward.x);
	Vector3 moveDirection = forward * leftStick.y + right * leftStick.x;

	/// === 移動入力があるなら正規化 === ///
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		Normalize(moveDirection);
	}

	/// === エネルギー未満なら少し加速 === ///
	float speed = (energyInfo_.rest < energyInfo_.max) ? moveInfo_.speed * 1.2f : moveInfo_.speed;

	translate_ += moveDirection * speed * deltaTime_;

	/// === 左スティック押し込みでブースト開始 === ///
	if (Input::TriggerButton(0, ControllerButtonType::LeftStick)) {
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
	rotate_.y += rightStick.x * boostInfo_.rotationSpeed;
	rotate_.x = std::clamp(rotate_.x + rightStick.y * boostInfo_.rotationSpeed, -boostInfo_.maxPitch, boostInfo_.maxPitch);

	/// ===左スティックの取得=== ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// === エネルギー消費 === ///
	energyInfo_.rest -= energyInfo_.drain * deltaTime_;

	/// === ブースト解除条件 === ///
	if (leftStick.y < -0.5f || Input::TriggerButton (0, ControllerButtonType::A) || energyInfo_.rest <= 0.0f) {
		behaviorRequest_ = Behavior::kRoot;
	}

	/// === `rotate_` に基づいて移動ベクトルを計算 (X, Y 軸を考慮) === ///
	Vector3 forward;
	forward.x = std::cos(rotate_.x) * std::sin(rotate_.y); // X軸の傾きも考慮
	forward.y = std::sin(-rotate_.x);                      // 上下移動も考慮
	forward.z = std::cos(rotate_.x) * std::cos(rotate_.y); // Z軸

	Normalize(forward); // 速度を一定にするため正規化

	/// === ブースト移動（`rotate_` に基づく方向に進む） === ///
	translate_ += forward * boostInfo_.speed * deltaTime_;
}
// カメラ
void Player::UpdateCamera() {
	/// === カメラオフセットの補間 === ///
	float targetBlend = (behavior_ == Behavior::kBoost) ? 1.0f : 0.0f;
	cameraInfo_.blend = Lerp(cameraInfo_.blend, targetBlend, cameraInfo_.lerpSpeed * deltaTime_);

	/// === オフセットの適用 === ///
	cameraInfo_.offset = Lerp(cameraInfo_.moveOffset, cameraInfo_.boostOffset, cameraInfo_.blend);
}
// ジャンプ
void Player::UpdateAir() {
	/// === Aボタンで上昇 === ///
	if (Input::PushButton(0, ControllerButtonType::A)) {
		jumpInfo_.verticalVelocity = jumpInfo_.verticalSpeed;
	} else {
		jumpInfo_.verticalVelocity += jumpInfo_.gravity * deltaTime_;
	}

	/// === 地面との衝突 === ///
	if (translate_.y + jumpInfo_.verticalVelocity * deltaTime_ < jumpInfo_.groundLevel) {
		translate_.y = jumpInfo_.groundLevel;
		jumpInfo_.verticalVelocity = 0.0f;
	} else {
		translate_.y += jumpInfo_.verticalVelocity * deltaTime_;
	}
}
