#include "SphereCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SphereCollider::Initialize() {
	type_ = ColliderType::Sphere;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SphereCollider::Update() {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawSphere(sphere_, lineColor_);

#endif // DEBUG

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SphereCollider::Draw(BlendMode mode) {
	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Radius
Sphere SphereCollider::GetSphere() const {
	return sphere_;
}
