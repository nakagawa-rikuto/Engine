#include "Player.h"

#include "Engine/Core/Mii.h"
#include "PlayerBullet.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {
	sprite_.reset();
	bullet_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialzie() {

	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();
	sprite_->SetTexture("./Resource/uvChecker.png");
	sprite_->SetPosition(postiton_);
	sprite_->SetSize(size_);
	sprite_->SetAnchorPoint(Vector2(0.5f, 0.5f));
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update() {

	/// ===Bullet処理=== ///
	if (bullet_) {
		bullet_->Update();
		if (!bullet_->GetFlag()) {
			bullet_.reset();
		}
	}

	/// ===発射処理=== ///
	if (Mii::TriggerKey(DIK_SPACE)) {
		if (!isShot_) {
			bullet_ = std::make_shared<PlayerBullet>();
			bullet_->Initialze(postiton_);
			isShot_ = true;
		}
	}

	/// ===移動処理=== ///
	if (Mii::PushKey(DIK_D)) {
		postiton_.x += 2.0f;
	} else if (Mii::PushKey(DIK_A)) {
		postiton_.x += -2.0f;
	}

	// spriteにセット
	sprite_->SetPosition(postiton_);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Player::Draw() {

	if (bullet_) {
		bullet_->Draw();
	}

	sprite_->Draw();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Vector2 Player::GetPosition() const { return postiton_; }
Vector2 Player::GetSize() const { return size_; }
Vector2 Player::GetBulletPos() const { return bullet_->GetPos(); }
Vector2 Player::GetBulletSize() const { return bullet_->GetSize(); }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void Player::SetPosition(const Vector2& pos) { postiton_ = pos; }
