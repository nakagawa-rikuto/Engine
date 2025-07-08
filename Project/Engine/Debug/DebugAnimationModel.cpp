#include "DebugAnimationModel.h"
#include "Engine/System/Service/InputService.h"

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
	if (InputService::TriggerKey(DIK_SPACE)) {
		if (isChagAnimation_) {
			// アニメーションのループを有効化
			object3d_->SetAnimation("Armature|mixamo.com|Layer0", false);
			isChagAnimation_ = false;
		} else {
			// アニメーションのループを無効化
			object3d_->SetAnimation("Armature|mixamo.com|Layer0", true);
			isChagAnimation_ = true;
		}
	}

	SetTranslate(base_.translate);
	SetRotate(base_.rotate);
	SetScale(base_.scale);
	SetColor(base_.color);

	object3d_->SetEnviromentMapData(true, 1.0f);

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
	ImGui::ColorEdit4("Color", &base_.color.x);
	SphereCollider::Info();
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突時の処理
///-------------------------------------------///
void DebugAnimationModel::OnCollision(Collider * collider) {

	if (collider->GetColliderName() == ColliderName::Enemy) {
		//base_.color = { 1.0f, 0.0f, 0.0f, 1.0f }; // 赤色に変更
	}
}
