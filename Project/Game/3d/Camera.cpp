#include "Camera.h"

#include "Engine/Core/WinApp.h"
#include "Math/sMath.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// WorldMatrix
const Matrix4x4& Camera::GetWorldMatrix() const { return worldMatrix_; }
// ViewMatrix
const Matrix4x4& Camera::GetViewMatrix() const { return viewMatrix_; }
// ProjectionMatrix
const Matrix4x4& Camera::GetProjectionMatrix() const { return projectionMatrix_; }
// ViewProjectionMatrix
const Matrix4x4& Camera::GetViewProjectionMatrix() const { return viewProjectionMatrix_; }
// Translate
const Vector3& Camera::GetTranslate() const { return transform_.translate; }
// Rotate
const Vector3& Camera::GetRotate() const { return transform_.rotate; }
// Scale
const Vector3& Camera::GetScale() const { return transform_.scale; }


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void Camera::SetTranslate(const Vector3& translate) { transform_.translate = translate; }
// Rotate
void Camera::SetRotate(const Vector3& rotate) { transform_.rotate = rotate; }
// Scale
void Camera::SetScale(const Vector3& scale) { transform_.scale = scale; }
// ForY
void Camera::SetForY(const float& forY) { horizontalView_ = forY; }
// AspectRatio
void Camera::SetAspectRatio(const float& aspect) { aspect_ = aspect; }
// NearClip
void Camera::SetNearClip(const float& nearClip) { nearClip_ = nearClip; }
// FarClip
void Camera::SetFarClip(const float& farClip) { farClip_ = farClip; }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Camera::Initialize() {
	transform_ = { {1.0f, 1.0f, 1.0f},{0.05f, 0.0f, 0.0f}, {0.0f, 7.0f, -40.0f} };
	horizontalView_ = 0.45f;
	aspect_ = static_cast<float>(WinApp::kWindowWidth) / static_cast<float>(WinApp::kWindowHeight);
	nearClip_ = 0.1f;
	farClip_ = 100.0f;
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse4x4(worldMatrix_);
	projectionMatrix_ = MakePerspectiveFovMatrix(horizontalView_, aspect_, nearClip_, farClip_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Camera::Update() {
	if (is2DMode_) {
		worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		viewMatrix_ = Inverse4x4(worldMatrix_);
	} else {
		worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
		viewMatrix_ = Inverse4x4(worldMatrix_);
		projectionMatrix_ = MakePerspectiveFovMatrix(horizontalView_, aspect_, nearClip_, farClip_);
	}
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

///-------------------------------------------/// 
/// 追加情報
///-------------------------------------------///
void Camera::InitializeFor2D() {
	WorldTransform transform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	float nearClip = -10.0f;
	float farClip = 10.0f;
	float left = 0.0f;
	float right = 50.0f;
	float bottom = 0.0f;
	float top = 50.0f;

	projectionMatrix_ = MakeOrthographicMatrix(left, right, bottom, top, nearClip, farClip);
	worldMatrix_ = MakeAffineMatrix(transform.scale, transform.rotate, transform.translate);
	viewMatrix_ = Inverse4x4(worldMatrix_);
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}
// 
void Camera::SwitchTo2DMode() {
	is2DMode_ = true;

	InitializeFor2D();
}
//
void Camera::SwitchTo3DMode() {
	is2DMode_ = false;

	Initialize();
}

