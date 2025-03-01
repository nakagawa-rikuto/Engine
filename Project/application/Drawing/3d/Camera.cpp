#include "Camera.h"

#include "Engine/Core/WinApp.h"
#include "Math/sMath.h"
#include "Math/EasingMath.h"

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
const Vector3& Camera::GetTranslate() const { return addTransform_.translate; }
// Rotate
const Vector3& Camera::GetRotate() const { return addTransform_.rotate; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void Camera::SetTranslate(const Vector3& translate) { addTransform_.translate = translate; }
// Rotate
void Camera::SetRotate(const Vector3& rotate) { addTransform_.rotate = rotate; }
// ForY
void Camera::SetForY(const float& forY) { horizontalView_ = forY; }
// AspectRatio
void Camera::SetAspectRatio(const float& aspect) { aspect_ = aspect; }
// NearClip
void Camera::SetNearClip(const float& nearClip) { nearClip_ = nearClip; }
// FarClip
void Camera::SetFarClip(const float& farClip) { farClip_ = farClip; }
// 追従対象の座標を設定
void Camera::SetTarget(Vector3* position, Vector3* rotation) {
	targetPos_ = position;
	targetRot_ = rotation;
}
// 追従のオフセット
void Camera::SetOffset(const Vector3& offset) { offset_ = offset; }
// 追従速度を設定
void Camera::SetFollowSpeed(float speed) { followSpeed_ = speed; }
// 回転補間速度
void Camera::SetLerpSpeed(float speed) { rotationLerpSpeed_ = speed; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Camera::Initialize() {
	transform_ = { {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.f} };
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

	if (targetPos_ && targetRot_) {
		// 追従処理
		//FollowTarget();
		PreFollowTarget();
	} else {
		transform_ = addTransform_;
	}

	// 行列の計算
	worldMatrix_ = MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = Inverse4x4(worldMatrix_);

	// プロジェクション行列の更新
	projectionMatrix_ = MakePerspectiveFovMatrix(horizontalView_, aspect_, nearClip_, farClip_);

	// 合成行列
	viewProjectionMatrix_ = Multiply(viewMatrix_, projectionMatrix_);
}

///-------------------------------------------/// 
/// 追従処理
///-------------------------------------------///
void Camera::FollowTarget() {
	/// ===プレイヤーの回転とカメラの回転を組み合わせる=== ///
	Vector3 combinedRotation = *targetRot_ + addTransform_.rotate;

	/// ===プレイヤーの回転に基づいてオフセットを回転=== ///
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(combinedRotation.y);
	Vector3 rotatedOffset = TransformVector(offset_, rotationMatrix);

	/// ===目標のカメラ位置を計算=== ///
	Vector3 targetCameraPos = *targetPos_ + rotatedOffset + addTransform_.translate;

	/// ===線形補間 (Lerp) を使って滑らかに追従=== ///
	transform_.translate = Lerp(transform_.translate, targetCameraPos, followSpeed_);

	/// ===カメラの向きをプレイヤーの回転に合わせる=== ///
	transform_.rotate = Lerp(transform_.rotate, combinedRotation, rotationLerpSpeed_);
}

/// ===追従処理=== ///
void Camera::PreFollowTarget() {
	if (targetPos_ && targetRot_) {
		// プレイヤーの回転に基づいてオフセットを回転
		Matrix4x4 rotationMatrix = MakeRotateYMatrix(targetRot_->y);
		Vector3 rotatedOffset = TransformVector(offset_, rotationMatrix);

		// 目標のカメラ位置を計算
		Vector3 targetCameraPos = *targetPos_ + rotatedOffset;

		// 線形補間 (Lerp) を使って滑らかに追従
		transform_.translate = Lerp(transform_.translate, targetCameraPos, followSpeed_);

		// カメラの向きをプレイヤーの位置に向ける
		transform_.rotate.y = targetRot_->y;
	}
}
