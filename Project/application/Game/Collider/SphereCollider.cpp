#include "SphereCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SphereCollider::Initialize() {
	type_ = ColliderType::Sphere;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Radius
Sphere SphereCollider::GetSphere() const {
	return sphere_;
}
