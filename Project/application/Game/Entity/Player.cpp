#include "Player.h"
// application
#include "application/Drawing/3d/Camera.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {
	model_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize(const std::string & modelName) {
	// model
	model_ = std::make_unique<Model>();
	model_->Initialize(modelName);  // 初期化(const std::string& modelNameが必須)
	model_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update(Camera * camera) {

	/// ===Behavior遷移の実装=== ///
	if (behaviorRequest_) {
		// 振る舞いを変更
		behavior_ = behaviorRequest_.value();

		// 各振る舞いの初期化
		switch (behavior_) {
		case Player::Behavior::kRoot:
			InitializeRoot();
			break;
		case Player::Behavior::kMove:
			InitializeMove();
			break;
		case Player::Behavior::kBoost:
			InitializeBoost();
			break;
		}

		// 振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 各振る舞いの更新
	switch (behavior_) {
	case Player::Behavior::kRoot:
		UpdateRoot();
		Jump();
		break;
	case Player::Behavior::kMove:
		UpdateMove();
		Jump();
		break;
	case Player::Behavior::kBoost:
		UpdateBoost();
		break;
	}

	model_->SetPosition(translate_);
	model_->SetRotate(rotate_);
	model_->SetScale(scale_);
	model_->SetCamera(camera);
	model_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Player::Draw() {
	model_->Draw();
}

///-------------------------------------------/// 
/// 初期化(Behavior)
///-------------------------------------------///
// 通常
void Player::InitializeRoot() {}
// 移動
void Player::InitializeMove() {}
// ブースト
void Player::InitializeBoost() {}

///-------------------------------------------/// 
/// 更新(Behavior)
///-------------------------------------------///
// 通常
void Player::UpdateRoot() {}
// 移動
void Player::UpdateMove() {}
// ブースト
void Player::UpdateBoost() {}
// ジャンプ
void Player::Jump() {}
