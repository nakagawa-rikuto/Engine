#include "OBBCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OBBCollider::Initialize() {
	type_ = ColliderType::OBB;
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
OBB OBBCollider::GetOBB() const {
	return obb_;
}
