#include "PlayerBullet.h"

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
PlayerBullet::~PlayerBullet() {
	sprite_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void PlayerBullet::Initialze(const Vector2& pos) {

	position_ = pos;

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();
	sprite_->SetTexture("./Resource/monsterBall.png");
	sprite_->SetPosition(position_);
	sprite_->SetSize(size_);
	sprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void PlayerBullet::Update() {
	
	if (isAlive_) {
		currentTimer_ += 1.0f / 60.0f;
	}
	
	if (currentTimer_ >= LifeTime_) {
		isAlive_ = false;
	}

	position_ -= velocity_;

	sprite_->SetPosition(position_);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void PlayerBullet::Draw() {

	sprite_->Draw();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
bool PlayerBullet::GetFlag() const { return isAlive_; }

Vector2 PlayerBullet::GetPos() const { return position_; }

Vector2 PlayerBullet::GetSize() const { return size_; }
