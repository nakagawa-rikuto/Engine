#include "OBBCollider.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OBBCollider::Initialize() {
	type_ = ColliderType::OBB;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void OBBCollider::Update() {

	// Line
#ifdef _DEBUG
	// デバッグ時のみ描画
	line_->DrawOBB(obb_, lineColor_);

#endif // DEBUG

	// Colliderの更新処理
	Collider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void OBBCollider::Draw(BlendMode mode) {
	// Colliderの描画処理
	Collider::Draw(mode);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
OBB OBBCollider::GetOBB() const {
	return obb_;
}
