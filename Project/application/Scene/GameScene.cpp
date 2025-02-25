#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// c++
#include <algorithm> 
// Service
#include "Engine/System/Service/Input.h"
// Math
#include "Math/sMath.h"

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
	ImGui::DragFloat("speed", &speed_, 0.01f);
	ImGui::DragFloat("boostMultiplier", &boostMultiplier_, 0.01f);
	ImGui::DragFloat("verticalSpeed", &verticalSpeed_, 0.01f);
	ImGui::DragFloat("gravity", &gravity_, 0.01f);
	ImGui::DragFloat("rotationSpeed", &rotationSpeed_, 0.01f);
	ImGui::DragFloat("maxPitch", &maxPitch_, 0.01f);
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
	FPSCamera_->SetOffset({ 0.0f, 0.3f, -7.0f });
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
	// 左スティックの入力取得（移動用）
	StickState leftStick = Input::GetLeftStickState(0);

	// 右スティックの入力取得（視点回転用）
	StickState rightStick = Input::GetRightStickState(0);

	// **視点（プレイヤーの向き）を更新**
	modelRotate_.y += rightStick.x * rotationSpeed_; // 左右回転（Yaw）
	modelRotate_.x = std::clamp(modelRotate_.x + rightStick.y * rotationSpeed_, -maxPitch_, maxPitch_); // 上下回転制限

	// **ブースト（RT または LT 押下で加速）**
	static bool isBoosting = false;
	if (Input::PushButton(0, ControllerButtonType::RT) || Input::PushButton(0, ControllerButtonType::LT)) {
		if (!isBoosting) {
			// ブースト開始時に現在の向きを基準にする
			boostDirection_ = modelRotate_.y;
			isBoosting = true;
		}
	} else {
		isBoosting = false;
	}

	// **ブースト制限（±90度以内）**
	if (isBoosting) {
		// 左スティックの移動角度を取得
		float inputAngle = std::atan2(leftStick.x, leftStick.y);

		// **入力角度とブースト開始時の角度の差を求める**
		float angleDifference = NormalizeAngle(inputAngle - boostDirection_);

		// **±90度を超えたらブースト解除**
		if (angleDifference < -(Pi() / 2.0f) || angleDifference >(Pi() / 2.0f)) {
			isBoosting = false;
		}
	}

	// **向いている方向の前方ベクトルを計算**
	float adjustedYaw = modelRotate_.y;

	// **ブースト中は22.5度刻みにスナップ**
	if (isBoosting) {
		adjustedYaw = SnapAngle(modelRotate_.y, 22.5f * (Pi() / 180.0f)); // 22.5度刻み
	}

	Vector3 forward(std::sin(adjustedYaw), 0.0f, std::cos(adjustedYaw)); // 前方ベクトル
	Vector3 right(forward.z, 0.0f, -forward.x); // 右方向ベクトル

	// **移動ベクトルを計算**
	Vector3 moveDirection = forward * leftStick.y + right * leftStick.x;

	// 移動入力があるかチェック
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		Normalize(moveDirection); // 正規化して一定の速度で移動
	}

	// **上昇処理（Aボタン）**
	if (Input::PushButton(0, ControllerButtonType::A)) {
		verticalVelocity_ = verticalSpeed_; // Aボタンで上昇
	} else {
		verticalVelocity_ += gravity_ * deltaTime_; // 重力による落下
	}

	// **地面との衝突処理**
	if (modelTranslate_.y + verticalVelocity_ * deltaTime_ < groundLevel_) {
		modelTranslate_.y = groundLevel_; // 地面に着地
		verticalVelocity_ = 0.0f;   // 落下を停止
	} else {
		modelTranslate_.y += verticalVelocity_ * deltaTime_; // Y座標を更新
	}

	// **プレイヤーの位置を更新**
	float boostMultiplier = isBoosting ? boostMultiplier_ : 1.0f;
	modelTranslate_ += moveDirection * speed_ * boostMultiplier; // 向いている方向に移動
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