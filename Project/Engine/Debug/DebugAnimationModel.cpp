#include "DebugAnimationModel.h"
#include "Engine/System/Service/Input.h"

#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
DebugAnimationModel::~DebugAnimationModel() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DebugAnimationModel::Initialize() {

	/// ===モデルを生成=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::AnimationModel, "human", LightType::Lambert);
	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);
	// アニメーションを登録しないとアニメーションが再生されない
	object3d_->SetAnimation("Armature|mixamo.com|Layer0");
	
	// コライダーの名前を登録
	name_ = ColliderName::Player;
	// Sphere情報を登録
	SphereCollider::Initialize();
	sphere_.radius = 0.8f;

	// 一回更新を行う
	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DebugAnimationModel::Update() {

	/// ===Animationの切り替え=== ///
	//if (Input::TriggerMouse(MouseButtonType::Right)) {
	//	// アニメーションのループを無効化
	//	object3d_->SetAnimation("Armature|mixamo.com|Layer0", true);
	//} else if (Input::TriggerMouse(MouseButtonType::Left)) {
	//	// アニメーションのループを有効化
	//	object3d_->SetAnimation("Armature|mixamo.com|Layer0", false);
	//}

	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);

	// SphereColliderの更新
	SphereCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DebugAnimationModel::Draw(BlendMode mode) {
	// SphereColliderの描画
	SphereCollider::Draw(mode);
}

///-------------------------------------------/// 
/// ImGuiの情報表示
///-------------------------------------------///
void DebugAnimationModel::ImGuiInfo() {
#ifdef USE_IMGUI
	ImGui::Begin("DebugAnimationModel");
	ImGui::DragFloat3("Transform", &base_.translate.x, 0.1f);
	ImGui::DragFloat4("Rotate", &base_.rotate.x, 0.01f);
	ImGui::DragFloat4("Color", &base_.color.x, 0.1f);
	ImGui::DragFloat("Radius", &sphere_.radius, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突時の処理
///-------------------------------------------///
void DebugAnimationModel::OnCollision(Collider * collider) {

	if (collider->GetColliderName() == ColliderName::Enemy) {
		base_.color = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤色に変更
	}
}
