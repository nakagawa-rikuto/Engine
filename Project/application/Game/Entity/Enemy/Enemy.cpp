
#include "Enemy.h"
// Camera
#include "application/Game/Camera/Camera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/ParticleService.h"
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Enemy::~Enemy() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
Vector3 Enemy::GetTranslate() const { return baseInfo_.translate; }
Quaternion Enemy::GetRotate() const { return baseInfo_.rotate; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Enemy::Init(Camera* camera, Player* player) {
	camera_ = camera;
	player_ = player;

	// 初期化
	Initialize();
	// Cameraの設定
	SetCamera(camera_);

	// object3dの更新を一回行う
	object3d_->Update();
}
void Enemy::Initialize() {

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall");
	// Object3dの初期設定
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetScale(baseInfo_.scale);
	SetColor(baseInfo_.color);

	// Sphereの設定
	SphereCollider::Initialize();
	sphere_.center = baseInfo_.translate;
	sphere_.radius = 2.0f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Enemy::Update() {


	//baseInfo_.color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);


	// 移動処理
	//Move();


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
void Enemy::Draw(BlendMode mode) {
	SphereCollider::Draw(mode);
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void Enemy::UpdateImGui() {
#ifdef _DEBUG
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &baseInfo_.rotate.x, 0.1f);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::End();
#endif // _DEBUG
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void Enemy::OnCollision(Collider* collider) {
	// Playerとの当たり判定
	if (collider->GetColliderName() == ColliderName::Player) {
		// Playerの突進に対しての衝突処理
		if (player_->GetCargeFlag()) {
			// パーティクルを発生
			// 現状課題ようで設定
			ParticleService::Emit("Explosion", baseInfo_.translate);
			//ServiceParticle::SetTexture("Cylinder", "gradationLine");
		}
	}
}

///-------------------------------------------/// 
/// 移動処理
///-------------------------------------------///
void Enemy::Move() {
	// 左スティック入力取得（移動用）
	StickState leftStick = InputService::GetRightStickState(0);

	// 方向の設定
	Vector3 direction = { 0.0f, 0.0f, 0.0f };
	direction.x = leftStick.x;
	direction.z = leftStick.y;

	// Velcotiyに反映
	baseInfo_.velocity = direction * 0.4f;
}
