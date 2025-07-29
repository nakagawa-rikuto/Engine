#include "AvoidanceState.h"
// Player, Camera
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Camera/Camera.h"
// State
#include "RootState.h"

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
AvoidanceState::AvoidanceState(const Vector3& direction) {info_.direction = direction;}

///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void AvoidanceState::Enter(Player * player, Camera * camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 回避情報の初期化
	info_.acceleration = 0.2f;
	info_.timer = info_.activeTime;
	player_->SetStateFlag(actionType::kAvoidance, true);
	player_->SetpreparationFlag(actionType::kAvoidance, false);
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void AvoidanceState::Update(Player * player, Camera * camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 無敵時間の変更
	player_->SetInvicibleTime(info_.invincibleTime);
	// 加速度の減少
	info_.acceleration -= player_->GetDeltaTime() * info_.activeTime;
	// 速度の設定
	info_.speed = 0.4f * info_.acceleration;

	// Velocityに反映
	player_->SetVelocityX(info_.direction.x * info_.speed);
	player_->SetVeloctiyZ(info_.direction.z * info_.speed);

	/// ===タイマーが時間を超えたら=== ///
	if (info_.timer <= 0.0f) {
		player_->ChangState(std::make_unique<RootState>());
		player_->SetStateFlag(actionType::kAvoidance, false);
		info_.timer = info_.cooltime; // クールタイムのリセット
	}
}

///-------------------------------------------/// 
/// 終了時に呼び出す
///-------------------------------------------///
void AvoidanceState::Finalize() {

}
