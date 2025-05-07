#include "Player.h"
// Service
#include "Engine/System/Service/Input.h"
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
Vector3 Player::GetRotate() const { return baseInfo_.rotate; }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize() {
	// Sphereの設定
	SphereCollider::Initialize();
	sphere_.center = baseInfo_.translate;
	sphere_.radius = 1.0f;

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall");
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetScale(baseInfo_.scale);
	SetColor(baseInfo_.color);
	object3d_->Update();
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
			// 通常
		case Player::Behavior::kRoot:
			InitRoot();
			break;
			// 移動
		case Player::Behavior::kMove:
			InitMove();
			break;
			// 突進
		case Player::Behavior::kCharge:
			InitCharge();
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
	case Player::Behavior::kCharge:
		UpdateCharge();
		break;
	}

	/// ===移動量の反映=== ///
	baseInfo_.translate += baseInfo_.velocity_;

	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	object3d_->Update();
}


///-------------------------------------------///  
/// 描画
///-------------------------------------------///
void Player::Draw() {
	object3d_->Draw();
}


///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void Player::UpdateImGui() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat3("Rotate", &baseInfo_.rotate.x, 0.1f);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity_.x, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
void Player::OnCollision(Collider * collider) {}


///-------------------------------------------/// 
/// Root
///-------------------------------------------///
void Player::InitRoot() {}
void Player::UpdateRoot() {

	// 左スティック入力取得（移動用）
	StickState leftStick = Input::GetLeftStickState(0);
	// 右スティック入力取得（視点回転用）
	StickState rightStick = Input::GetRightStickState(0);

	/// ===Behaviorの遷移=== ///
	// 移動があれば移動状態へ
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {
		// behaviorRequest_ = Behavior::kRotateToCamera;
		behaviorRequest_ = Behavior::kMove;
	}
}

///-------------------------------------------/// 
/// Move
///-------------------------------------------///
void Player::InitMove() {
	moveInfo_.speed = 0.4f;
}
void Player::UpdateMove() {

	// 左スティック入力取得（移動用）
	StickState leftStick = Input::GetLeftStickState(0);
	// 右スティック入力取得（視点回転用）
	StickState rightStick = Input::GetRightStickState(0);

	baseInfo_.velocity_.x = leftStick.x;
	baseInfo_.velocity_.z = leftStick.y;
	
	baseInfo_.velocity_ = baseInfo_.velocity_ * moveInfo_.speed;

	/// ===Behaviorの変更=== ///
	if (std::abs(leftStick.x) < 0.1f && std::abs(leftStick.y) < 0.1f) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

///-------------------------------------------/// 
/// Charge
///-------------------------------------------///
void Player::InitCharge() {}
void Player::UpdateCharge() {}
