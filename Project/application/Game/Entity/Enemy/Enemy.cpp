#include "Enemy.h"
// Camera
#include "application/Game/Camera/Camera.h"
// Service
#include "Engine/System/Service/Input.h"
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

Enemy::~Enemy() {
	object3d_.reset();
}

void Enemy::Init(Camera * camera) {
	camera_ = camera;

	// 初期化
	Initialize();
	// Cameraの設定
	SetCamera(camera_);

	// object3dの更新を一回行う
	object3d_->Update();
}

void Enemy::Initialize() {
	// Sphereの設定
	SphereCollider::Initialize();
	sphere_.center = baseInfo_.translate;
	sphere_.radius = 1.0f;

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall");
	// Object3dの初期設定
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	SetScale(baseInfo_.scale);
	SetColor(baseInfo_.color);
}

void Enemy::Update() {

	// 移動処理
	Move();

	/// ===移動量の反映=== ///
	baseInfo_.translate += baseInfo_.velocity;

	/// ===Object3dの更新=== ///
	SetTranslate(baseInfo_.translate);
	SetRotate(baseInfo_.rotate);
	
	// SphereColliderの更新
	SphereCollider::Update();
}

void Enemy::Draw(BlendMode mode) {
	SphereCollider::Draw(mode);
}

void Enemy::UpdateImGui() {
	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Translate", &baseInfo_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &baseInfo_.rotate.x, 0.1f);
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::End();
}

void Enemy::OnCollision(Collider * collider) {}

Vector3 Enemy::GetTranslate() const { return baseInfo_.translate; }

Quaternion Enemy::GetRotate() const { return baseInfo_.rotate; }

void Enemy::Move() {
	// 左スティック入力取得（移動用）
	StickState leftStick = Input::GetRightStickState(0);

	// 方向の設定
	Vector3 direction = { 0.0f, 0.0f, 0.0f };
	direction.x = leftStick.x;
	direction.z = leftStick.y;

	// Velcotiyに反映
	baseInfo_.velocity = direction * 0.4f;
}
