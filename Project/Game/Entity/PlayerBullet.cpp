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
	velocity_ = Vector2(0.0f, 1.0f);

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();
	sprite_->SetTexture("./Resource/monsterBall.png");
	sprite_->SetPosition(position_);
	sprite_->SetSize(Vector2(30.0f, 30.0f));
	sprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void PlayerBullet::Update() {
	position_ += velocity_;

	sprite_->SetPosition(position_);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void PlayerBullet::Draw() {

	sprite_->Draw();
}
