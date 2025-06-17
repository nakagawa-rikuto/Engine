#include "SphereCollider.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SphereCollider::Initialize() {
	type_ = ColliderType::Sphere;

	sphere_.center = object3d_->GetTranslate();

	Collider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SphereCollider::Update() {

	// センター座標を更新
	sphere_.center = object3d_->GetTranslate();

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SphereCollider::Draw(BlendMode mode) {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawSphere(sphere_, lineColor_);
#endif // DEBUG

	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// スフィア情報のImGui
///-------------------------------------------///
void SphereCollider::SphereInfo() {
#ifdef USE_IMGUI
	ImGui::Text("SphereInfo");
	ImGui::DragFloat("radius", &sphere_.radius, 0.1f);
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Radius
Sphere SphereCollider::GetSphere() const {
	return sphere_;
}

