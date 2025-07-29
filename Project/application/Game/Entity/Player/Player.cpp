#include "Player.h"
// Camera
#include "application/Game/Camera/Camera.h"
// State
#include "State/RootState.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/CameraService.h"
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
/// Getter
///-------------------------------------------///
// 座標
Vector3 Player::GetTranslate() const { return baseInfo_.translate; }
// 回転
Quaternion Player::GetRotate() const { return baseInfo_.rotate; }
// 移動量
Vector3 Player::GetVelocity() const { return baseInfo_.velocity; }
// DeltaTime
float Player::GetDeltaTime() const { return deltaTime_; }
// フラグ
bool Player::GetStateFlag(actionType type) const {
	if (type == actionType::kAvoidance) {
		return avoidanceInfo_.isFlag;
	} else if (type == actionType::kCharge) {
		return chargeInfo_.isFlag;
	} else {
		return attackInfo_.isFlag;
	}
}
bool Player::GetpreparationFlag(actionType type) const {
	if (type == actionType::kAvoidance) {
		return avoidanceInfo_.isPreparation;
	} else if (type == actionType::kCharge) {
		return chargeInfo_.isPreparation;
	} else {
		return attackInfo_.isPreparation;
	}
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// 座標
void Player::SetTranslate(const Vector3& translate) { baseInfo_.translate = translate;}
// 回転
void Player::SetRotate(const Quaternion& rotate) { baseInfo_.rotate = rotate; }
// 移動量
void Player::SetVelocity(const Vector3& velocity) {baseInfo_.velocity = velocity;}
// 移動量に加算
void Player::SetVelocityX(const float& x) { baseInfo_.velocity.x += x; }
void Player::SetVeloctiyY(const float& y) { baseInfo_.velocity.y += y; }
void Player::SetVeloctiyZ(const float& z) { baseInfo_.velocity.z += z; }
// フラグ
void Player::SetStateFlag(actionType type, bool falg) {
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.isFlag = falg;
		break;
	case actionType::kCharge:
		chargeInfo_.isFlag = falg;
		break;
	case actionType::kAttack:
		attackInfo_.isFlag = falg;
		break;
	}
}
void Player::SetpreparationFlag(actionType type, bool falg) {
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.isPreparation = falg;
		break;
	case actionType::kCharge:
		chargeInfo_.isPreparation = falg;
		break;
	case actionType::kAttack:
		attackInfo_.isPreparation = falg;
		break;
	}
}
// タイマーの設定
void Player::SetTimer(actionType type, const float& timer) {
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.timer = timer;
		break;
	case actionType::kCharge:
		chargeInfo_.timer = timer;
		break;
	case actionType::kAttack:
		attackInfo_.timer = timer;
		break;
	}
}
void Player::SetInvicibleTime(const float& time) { 
	invicibleInfo_.timer = invicibleInfo_.time + time; 
}


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize() {
	// Cameraの設定
	camera_ = CameraService::GetActiveCamera().get();

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");

	// Sphereの設定
	OBBCollider::Initialize();
	name_ = ColliderName::Player;
	obb_.halfSize = { 1.5f, 1.5f, 1.5f }; 

	// 初期設定
	ChangState(std::make_unique<RootState>());

	// 更新
	object3d_->Update();
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update() {
	/// ===スティックの取得=== ///
	StickState rightStick = InputService::GetRightStickState(0);
	camera_->SetStick({ rightStick.x, rightStick.y });

	/// ===タイマーを進める=== ///
	advanceTimer();

	/// ===Stateの管理=== ///
	if (currentState_) {
		// 各Stateの更新
		currentState_->Update(this, camera_);
	}
	

	/// ===移動量の反映=== ///
	baseInfo_.translate += baseInfo_.velocity;

	/// ===camera=== ///
	camera_->SetTarget(&baseInfo_.translate, &baseInfo_.rotate);

	/// ===Object3dの更新=== ///
	object3d_->SetTranslate(baseInfo_.translate);
	object3d_->SetRotate(baseInfo_.rotate);
	object3d_->SetScale(baseInfo_.scale);
	object3d_->SetColor(baseInfo_.color);

	/// ===SphereColliderの更新=== ///
	OBBCollider::Update();
}


///-------------------------------------------///  
/// 描画
///-------------------------------------------///
void Player::Draw(BlendMode mode) {
	/// ===SphereColliderの描画=== ///
	OBBCollider::Draw(mode);
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
void Player::OnCollision(Collider* collider) {
	// Colliderによって処理を変更
	if (collider->GetColliderName() == ColliderName::Enemy) {

		if (chargeInfo_.isFlag) {


		} else if (attackInfo_.isFlag) {


		} else {


		}
	}
}

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
		chargeInfo_.isPreparation = true;
	}

	// 回避用タイマーを進める
	if (avoidanceInfo_.timer > 0.0f) {
		avoidanceInfo_.timer -= deltaTime_;
	} else {
		avoidanceInfo_.isPreparation = true;
	}
}

///-------------------------------------------/// 
/// 減速処理
///-------------------------------------------///
void Player::ApplyDeceleration(const float& develeration) {
	// Velocityが0でないなら徐々に0にする
	if (baseInfo_.velocity.x != 0.0f) {
		// 各軸に対して減速適用
		baseInfo_.velocity.x *= develeration;
		// 小さすぎる値は完全に0にスナップ
		if (std::abs(baseInfo_.velocity.x) < 0.01f) {
			baseInfo_.velocity.x = 0.0f;
		}
	}
	if (baseInfo_.velocity.z != 0.0f) {
		baseInfo_.velocity.z *= develeration;
		if (std::abs(baseInfo_.velocity.z) < 0.01f) {
			baseInfo_.velocity.z = 0.0f;
		}
	}
}

///-------------------------------------------/// 
/// Stateの変更
///-------------------------------------------///
void Player::ChangState(std::unique_ptr<PlayerState> newState) {
	if (currentState_) {
		// 古い状態を解放  
		currentState_->Finalize();
		currentState_.reset();
	}

	// 新しい状態をセット  
	currentState_ = std::move(newState);
	// 新しい状態の初期化  
	currentState_->Enter(this, camera_);
}
