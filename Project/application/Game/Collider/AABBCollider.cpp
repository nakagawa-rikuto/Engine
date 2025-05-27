#include "AABBCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AABBCollider::Initialize() {
	type_ = ColliderType::AABB;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AABBCollider::Update() {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawAABB(aabb_, lineColor_);

#endif // DEBUG

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void AABBCollider::Draw(BlendMode mode) {
	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
AABB AABBCollider::GetAABB() const {
	return aabb_;
}
