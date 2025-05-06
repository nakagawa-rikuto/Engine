#include "Collider.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Type
ColliderType Collider::GetColliderType() { return type_; }
// Name
std::string Collider::GetColliderName() { return colliderName_; }



///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Object3D
void Collider::SetTranslate(const Vector3& translate) { object3d_->SetTranslate(translate); }
void Collider::SetRotate(const Vector3& rotate) { object3d_->SetRotate(rotate); }
void Collider::SetScale(const Vector3& scale) { object3d_->SetScale(scale); }
void Collider::SetColor(const Vector4& color) { object3d_->SetColor(color); }
// Camera
void Collider::SetCamera(Camera* camera) { object3d_->SetCamera(camera); }
// Light
void Collider::SetLight(LightType type) { object3d_->SetLight(type); }
void Collider::SetLightData(LightInfo light) { object3d_->SetLightData(light); }
