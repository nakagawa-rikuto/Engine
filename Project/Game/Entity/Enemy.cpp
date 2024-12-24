#include "Enemy.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Enemy::~Enemy() { sprite_.reset(); }

///-------------------------------------------/// 
///　初期化
///-------------------------------------------///
void Enemy::Initialize() {

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();
	sprite_->SetTexture("./Resource/checkerBoard.png");
	sprite_->SetPosition(postition_);
	sprite_->SetSize(size_);
	sprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Enemy::Update() {


}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Enemy::Draw() {
	sprite_->Draw();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Vector2 Enemy::GetPos() const { return postition_; }
Vector2 Enemy::GetSize() const { return size_; }
