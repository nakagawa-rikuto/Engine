#include "BaseEnemy.h"
// Camera
#include "application/Game/Camera/Camera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/CameraService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BaseEnemy::~BaseEnemy() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Vector3 BaseEnemy::GetTranslate() const { return baseInfo_.translate; }
Quaternion BaseEnemy::GetRotate() const { return baseInfo_.rotate; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void BaseEnemy::SetPlayer(Player* player) { player_ = player; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void BaseEnemy::Initialize() {

	// カメラの取得
	camera_ = CameraService::GetActiveCamera().get();

	// Cameraの設定
	SetCamera(camera_);

	// object3dの更新を一回行う
	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void BaseEnemy::Update() {

	/// ===移動量の反映=== ///
	baseInfo_.translate += baseInfo_.velocity;

	/// ===Object3dの更新=== ///
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetColor(baseInfo_.color);

	// SphereColliderの更新
	SphereCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BaseEnemy::Draw(BlendMode mode) {
	SphereCollider::Draw(mode);
}