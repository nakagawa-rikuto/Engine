#include "Player.h"

#include "Engine/Core/Mii.h"
#include "application/Game/Stage/Block.h"

#ifdef _DEBUG
#include "imgui.h"
#endif // _DEBUG


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// ポジション
const Vector3 Player::GetPos() { return pos_; }
// 半径
const float Player::GetRadius() { return radius_; }
// Blockのポジション
const Vector3 Player::GetBlockPos() { return blockPos_; }
// isCollsiion
bool Player::GetIsCollision() { return isCollision_; }



///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
//
void Player::SetPosititonZ(const float& posZ) { pos_.z = posZ; }
//
void Player::SetBlockPosition(Block* block) { blockPos_ = block->GetPos(); }
// 
void Player::NotCollisision() { isCollision_ = false; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Inititalze(const std::string & modelName, Camera* camera) {

	/// ===Transform情報=== ///
	pos_ = { 0.0f, 3.0f, 0.0f };
	radius_ = 0.3f;

	/// ===Model=== ///
	model_ = std::make_unique<Model>();
	model_->Initialize(modelName);
	model_->SetTransform(pos_, { 0.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 0.0f});
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update(Camera * camera, bool Mode) {


	if (!isCollision_) {
		pos_.y -= 0.4f;
	}

	if (Mode) {
		// 移動処理
		Move3D();
	} else {
		// 移動処理
		Move2D();
	}

	// モデルの更新
	model_->SetTransform(pos_, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	model_->SetCamera(camera);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Player::Draw() {
	model_->Draw();
}

///-------------------------------------------/// 
/// 衝突時の処理
///-------------------------------------------///
void Player::OnCollision(Block* block) {
	//衝突状態にする
	isCollision_ = true;

	// Blockの中心座標を取得
	Vector3 blockCenter = block->GetPos();

	// Y軸の押し戻し量を計算
	float pushBackY = pos_.y - blockCenter.y;

	// Y軸だけ押し戻す
	if (pushBackY > 0.0f) { // プレイヤーがブロックの上にいる場合
		pos_.y += 0.1f; // 0.1f は押し戻し量
	} else if (pushBackY < 0.0f) { // プレイヤーがブロックの下にいる場合
		pos_.y -= 0.1f; // 逆方向に押し戻し
	}
}

///-------------------------------------------/// 
/// 移動処理
///-------------------------------------------///
// 3D
void Player::Move3D() {

	/// ===入力処理=== ///
	// ｚ軸移動
	if (Mii::PushKey(DIK_W)) {
		pos_.z += 0.4f;
	} else if (Mii::PushKey(DIK_S)) {
		pos_.z -= 0.4f;
	}
	// ｘ軸移動
	if (Mii::PushKey(DIK_D)) {
		pos_.x += 0.4f;
	} else if (Mii::PushKey(DIK_A)) {
		pos_.x -= 0.4f;
	}

}
// 2D
void Player::Move2D() {

	/// ===入力処理=== ///
	// ｘ軸移動
	if (Mii::PushKey(DIK_D)) {
		pos_.x += 0.4f;
	} else if (Mii::PushKey(DIK_A)) {
		pos_.x -= 0.4f;
	}
}
