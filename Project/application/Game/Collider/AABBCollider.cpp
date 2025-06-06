#include "AABBCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AABBCollider::Initialize() {
	type_ = ColliderType::AABB;

	// Colliderの初期化
	Collider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AABBCollider::Update() {

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void AABBCollider::Draw(BlendMode mode) {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawAABB(aabb_, lineColor_);

#endif // DEBUG

	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
AABB AABBCollider::GetAABB() const {
	return aabb_;
}
