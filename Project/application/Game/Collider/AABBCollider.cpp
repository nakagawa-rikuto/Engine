#include "AABBCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AABBCollider::Initialize() {
	type_ = ColliderType::AABB;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
AABB AABBCollider::GetAABB() const {
	return aabb_;
}
