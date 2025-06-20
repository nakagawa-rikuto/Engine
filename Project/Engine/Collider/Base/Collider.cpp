#include "Collider.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Type
ColliderType Collider::GetColliderType() { return type_; }
// Name
ColliderName Collider::GetColliderName() { return name_; }


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Object3D
void Collider::SetTranslate(const Vector3& translate) { object3d_->SetTranslate(translate); }
void Collider::SetRotate(const Quaternion& rotate) { object3d_->SetRotate(rotate); }
void Collider::SetScale(const Vector3& scale) { object3d_->SetScale(scale); }
void Collider::SetColor(const Vector4& color) { object3d_->SetColor(color); }
// Camera
void Collider::SetCamera(Camera* camera) { object3d_->SetCamera(camera); }
// Light
void Collider::SetLight(LightType type) { object3d_->SetLight(type); }
void Collider::SetLightData(LightInfo light) { object3d_->SetLightData(light); }

// IsCollision
void Collider::SetIsCollisison(bool flag) { isCollision_ = flag; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Collider::Initialize() {
#ifdef _DEBUG
	line_ = std::make_unique<Line>();
#endif // _DEBUG

}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void Collider::Update() {

#ifdef _DEBUG
	// 線の色を更新
	if (isCollision_) {
		lineColor_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	} else {
		lineColor_ = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	}
#endif // _DEBUG	

	// Object3Dの更新
	object3d_->Update();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void Collider::Draw(BlendMode mode) {
	// Object3Dの描画
	object3d_->Draw(mode);
}
