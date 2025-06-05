#include "Player.h"
// Camera
#include "application/Game/Camera/Camera.h"
// Service
#include "Engine/System/Service/Input.h"
// Math
#include "Math/sMath.h"

// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
Vector3 Player::GetTranslate() const { return baseInfo_.translate; }
Quaternion Player::GetRotate() const { return baseInfo_.rotate; }

///-------------------------------------------/// 
/// 初期化（GameScene用）
///-------------------------------------------///
void Player::Init(Camera* camera) {
	
	camera_ = camera;

	// 初期化
	Initialize();
	// Cameraの設定
	SetCamera(camera_);
	camera_->SetFollowCamera(FollowCameraType::TopDown);
	camera_->SetOffset({ 0.0f, 70.0f, 0.0f });
	camera_->SetFollowSpeed(0.1f);
	
	object3d_->Update();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize() {

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetScale(baseInfo_.scale);
	SetColor(baseInfo_.color);

	// Sphereの設定
	SphereCollider::Initialize();
	sphere_.radius = 2.0f;
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update() {

	StickState rightStick = Input::GetRightStickState(0);
	camera_->SetStick({ rightStick.x, rightStick.y });

	/// ===タイマーを進める=== ///
	advanceTimer();

	/// ===Behavior遷移の実装=== ///
	if (behaviorRequest_) {
		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();
		// 各振る舞いの初期化
		switch (behavior_) {
			// 通常
		case Player::Behavior::kRoot:
			InitRoot();
			break;
			// 移動
		case Player::Behavior::kMove:
			InitMove();
			break;
		case Player::Behavior::kAvoidance:
			InitAvoidance();
			break;
			// 突進
		case Player::Behavior::kCharge:
			InitCharge();
			break;
		case Player::Behavior::kAttack:
			InitAttack();
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いの更新
	switch (behavior_) {
	case Player::Behavior::kRoot:
		UpdateRoot();
		break;
	case Player::Behavior::kMove:
		UpdateMove();
		break;
	case Player::Behavior::kAvoidance:
		UpdateAvoidance();
		break;
	case Player::Behavior::kCharge:
		UpdateCharge();
		break;
	case Player::Behavior::kAttack:
		UpdateAttack();
		break;
	}

	/// ===移動量の反映=== ///
	baseInfo_.translate += baseInfo_.velocity;

	/// ===camera=== ///
	camera_->SetTarget(&baseInfo_.translate, &baseInfo_.rotate);

	/// ===Object3dの更新=== ///
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetColor(baseInfo_.color);

	// SphereColliderの更新
	SphereCollider::Update();
}


///-------------------------------------------///  
/// 描画
///-------------------------------------------///
void Player::Draw(BlendMode mode) {
	// SphereCollider
	SphereCollider::Draw(mode);
}


///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void Player::UpdateImGui() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &baseInfo_.rotate.x, 0.1f);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::ColorEdit4("Color", &baseInfo_.color.x);
	ImGui::End();
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
void Player::OnCollision(Collider* collider) {}


///-------------------------------------------/// 
/// Root（通常）
///-------------------------------------------///
void Player::InitRoot() {}
void Player::UpdateRoot() {

	/// === 左スティック入力取得（移動用）=== ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// === 減速率（数値を下げればゆっくり止まる）=== ///
	const float deceleration = 0.75f;

	/// === 徐々に止まる処理 === ///
	// Velocityが0でないなら徐々に0にする
	if (baseInfo_.velocity.x != 0.0f) {
		// 各軸に対して減速適用
		baseInfo_.velocity.x *= deceleration;
		// 小さすぎる値は完全に0にスナップ
		if (std::abs(baseInfo_.velocity.x) < 0.01f) {
			baseInfo_.velocity.x = 0.0f;
		}
	}
	if (baseInfo_.velocity.z != 0.0f) {
		baseInfo_.velocity.z *= deceleration;
		if (std::abs(baseInfo_.velocity.z) < 0.01f) {
			baseInfo_.velocity.z = 0.0f;
		}
	}

	/// === Behaviorの遷移 === ///
	// 移動があれば移動状態へ
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		behaviorRequest_ = Behavior::kMove;
	}
	// Aボタンが押されたら進んでいる突進状態へ
	if (Input::TriggerButton(0, ControllerButtonType::A)) {
		// タイマーがクールタイムより高ければ、
		if (chargeInfo_.isFlag) {
			behaviorRequest_ = Behavior::kCharge;
			chargeInfo_.direction = Normalize(baseInfo_.velocity);
		}
	// 回避状態へ
	} else if (Input::TriggerButton(0, ControllerButtonType::LeftStick)) {
		if (avoidanceInfo_.isFlag) {
			behaviorRequest_ = Behavior::kAvoidance;
			avoidanceInfo_.direction = Normalize(moveInfo_.direction);
		}
	}
}

///-------------------------------------------/// 
/// Move（移動）
///-------------------------------------------///
void Player::InitMove() {
	moveInfo_.speed = 0.4f;
}
void Player::UpdateMove() {

	/// === 左スティック入力取得（移動用）=== ///
	StickState leftStick = Input::GetLeftStickState(0);

	/// ===移動処理=== ///
	// 方向の設定
	moveInfo_.direction.x = leftStick.x;
	moveInfo_.direction.z = leftStick.y;

	// Velcotiyに反映
	baseInfo_.velocity = moveInfo_.direction * moveInfo_.speed;

	/// ===移動方向に沿って回転=== ///
	// 方向が変更されたら
	if (Length(moveInfo_.direction) > 0.01f) {
		// 現在のYaw角（Y軸の回転）を取得
		float currentYaw = Math::GetYAngle(baseInfo_.rotate);

		// 入力方向から目標のYaw角を取得
		float targetYaw = std::atan2(moveInfo_.direction.x, moveInfo_.direction.z);

		// 差分を [-π, π] に正規化
		float diff = Math::NormalizeAngle(targetYaw - currentYaw);

		// イージング補間（短い方向へ回転）
		float easedYaw = currentYaw + diff * (deltaTime_ * 5.0f);

		// Quaternionに再変換
		baseInfo_.rotate = Math::MakeRotateAxisAngle({ 0, 1, 0 }, easedYaw);
	}

	/// ===Behaviorの変更=== ///
	if (std::abs(leftStick.x) < 0.1f && std::abs(leftStick.y) < 0.1f) {
		behaviorRequest_ = Behavior::kRoot;
	}
	if (Input::TriggerButton(0, ControllerButtonType::A)) {
		if (chargeInfo_.isFlag) {
			behaviorRequest_ = Behavior::kCharge;
			chargeInfo_.direction = Normalize(moveInfo_.direction);
		}
	} 
}

///-------------------------------------------/// 
/// Avoidance（回避）
///-------------------------------------------///
void Player::InitAvoidance() {
	// 回避スピードの設定
	avoidanceInfo_.acceleration = 0.3f;
	// 回避時間を0にする
	avoidanceInfo_.timer = avoidanceInfo_.activeTime;
}
void Player::UpdateAvoidance() {

	/// ===フラグがtruenなら=== ///
	// 無敵時間の変更
	invicibleInfo_.timer = invicibleInfo_.time - avoidanceInfo_.invincibleTime;

	// 加速度の減少
	avoidanceInfo_.acceleration -= deltaTime_ * avoidanceInfo_.activeTime;
	// 速度の設定
	avoidanceInfo_.speed = moveInfo_.speed * avoidanceInfo_.acceleration;

	// Velocityに反映
	baseInfo_.velocity += avoidanceInfo_.direction * avoidanceInfo_.speed;

	/// ===タイマーが回避の有効期限を超えていたら=== ///
	if (avoidanceInfo_.timer <= 0.0f) {
		behaviorRequest_ = Behavior::kRoot;
		avoidanceInfo_.isFlag = false;
		avoidanceInfo_.timer = avoidanceInfo_.cooltime;
	}
}

///-------------------------------------------/// 
/// Charge（突進マーク付け）
///-------------------------------------------///
void Player::InitCharge() {
	// 突進スピードの設定
	chargeInfo_.acceleration = 0.2f;
	// 突進時間を0にする
	chargeInfo_.timer = chargeInfo_.activeTime;
}
void Player::UpdateCharge() {

	/// ===フラグがtrueなら=== ///
	// 無敵時間の変更
	invicibleInfo_.timer = invicibleInfo_.time - chargeInfo_.invincibleTime;

	// 加速度の減少
	chargeInfo_.acceleration -= deltaTime_ * chargeInfo_.activeTime;
	// 速度の設定
	chargeInfo_.speed = moveInfo_.speed * chargeInfo_.acceleration;

	// Velcotiyに反映
	baseInfo_.velocity += chargeInfo_.direction * chargeInfo_.speed;

	/// ===タイマーが突進の有効期限を超えていたら=== ///
	if (chargeInfo_.timer <= 0.0f) {
		behaviorRequest_ = Behavior::kRoot;
		chargeInfo_.isFlag = false;
		chargeInfo_.timer = chargeInfo_.cooltime;
	}

}

///-------------------------------------------/// 
/// Attack（攻撃）
///-------------------------------------------///
void Player::InitAttack() {}
void Player::UpdateAttack() {}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void Player::advanceTimer() {
	// 無敵タイマーを進める
	invicibleInfo_.timer += deltaTime_;

	// 突進用のタイマーを進める
	if (chargeInfo_.timer > 0.0f) {
		chargeInfo_.timer -= deltaTime_;
	} else {
		chargeInfo_.isFlag = true;
	}

	// 回避用タイマーを進める
	if (avoidanceInfo_.timer > 0.0f) {
		avoidanceInfo_.timer -= deltaTime_;
	} else {
		avoidanceInfo_.isFlag = true;
	}
}
