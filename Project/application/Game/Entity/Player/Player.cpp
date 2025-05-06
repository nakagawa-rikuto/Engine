#include "Player.h"

// Service
#include "Engine/System/Service/Input.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize() {

	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall", LightType::None);
	object3d_->SetTranslate(translate_);
	object3d_->SetRotate(rotate_);
	object3d_->SetScale(scale_);
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
			InitRoot();
			break;
		case Player::Behavior::kMove:
			InitMove();
			break;
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

	// 座標に移動量を加算
	translate_ += velocity_;

	// Object3Dの更新
	object3d_->SetTranslate(translate_);
	object3d_->SetRotate(rotate_);
	object3d_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Player::Draw() {
	// BlendModeを変更可
	object3d_->Draw();
}

///-------------------------------------------/// 
/// 衝突時の応答処理
///-------------------------------------------///
void Player::OnCollision(Collider * collider) {}

///-------------------------------------------/// 
/// 振る舞い（kRoot）
///-------------------------------------------///
void Player::InitRoot() {}
void Player::UpdateRoot() {

	// スティックの値を取得
	StickState leftStick = Input::GetLeftStickState(0);
	StickState rightStick = Input::GetRightStickState(0);

	// ===Behaviorの遷移=== ///
	// 移動があれば移動状態へ
	if (std::abs(leftStick.x) > 0.1f || std::abs(leftStick.y) > 0.1f) {

		nextBehavior_ = Behavior::kMove;
		// behaviorRequest_ = Behavior::kRotateToCamera;

		behaviorRequest_ = Behavior::kMove;
	}
}

///-------------------------------------------/// 
/// 振る舞い（kMove）
///-------------------------------------------///
void Player::InitMove() {}
void Player::UpdateMove() {

	// スティックの値を取得
	StickState leftStick = Input::GetLeftStickState(0);
	StickState rightStick = Input::GetRightStickState(0);

	// スティックの値を取得
	velocity_.x = leftStick.x * moveInfo_.speed_;
	velocity_.z = leftStick.y * moveInfo_.speed_;	

	velocity_ = velocity_ * deltaTime_;
}

///-------------------------------------------/// 
/// 振る舞い（kCharge）
///-------------------------------------------///
void Player::InitCharge() {
	chargeInfo_.isActive_ = true;
}
void Player::UpdateCharge() {
	//NOTE:突撃時の動きはイージングで管理し、始まりから終わるまでに一回全店するみたいな感じにする

}
