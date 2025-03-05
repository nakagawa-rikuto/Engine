#include "Camera.h"

#include "Engine/Core/WinApp.h"
#include "Math/sMath.h"
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// FollowCameraの設定
///-------------------------------------------///
void Camera::SetFollowCamera(FollowCameraType type) {
	cameraType_ = type;
}

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
		//PreFollowTarget();
		UpdateFollowCamera();
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

///-------------------------------------------/// 
///
///-------------------------------------------///
void Camera::UpdateFollowCamera() {
	switch (cameraType_) {
	case FollowCameraType::FixedOffset:
		FollowFixedOffset();
		break;
	case FollowCameraType::Interpolated:
		FollowInterpolated();
		break;
	case FollowCameraType::Orbiting:
		FollowOrbiting();
		break;
	case FollowCameraType::CollisionAvoidance:
		FollowCollisionAvoidance();
		break;
	}
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void Camera::FollowFixedOffset() {
	// プレイヤーの回転を基にY軸回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(targetRot_->y);

	// オフセットを回転行列で変換し、適切な位置に調整
	Vector3 rotatedOffset = TransformVector(offset_, rotationMatrix);

	// 目標位置にオフセットを適用
	transform_.translate = *targetPos_ + rotatedOffset;

	// プレイヤーの回転と同じ向きを維持
	transform_.rotate = *targetRot_;
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void Camera::FollowInterpolated() {
	// プレイヤーの回転を基にY軸回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(targetRot_->y);

	// オフセットを回転行列で変換し、適切な位置に調整
	Vector3 rotatedOffset = TransformVector(offset_, rotationMatrix);

	// 目標位置を算出
	Vector3 targetCameraPos = *targetPos_ + rotatedOffset;

	// カメラの位置を補間（Lerp）で滑らかに移動
	transform_.translate = Lerp(transform_.translate, targetCameraPos, followSpeed_);

	// カメラの回転もプレイヤーの回転に向かって補間
	transform_.rotate = Lerp(transform_.rotate, *targetRot_, rotationLerpSpeed_);
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void Camera::FollowOrbiting() {
	float orbitAngleY = addTransform_.rotate.y;

	// プレイヤーの周囲を円軌道で回る
	float radius = 10.0f; // カメラの回転半径
	float camX = targetPos_->x + radius * cosf(orbitAngleY);
	float camZ = targetPos_->z + radius * sinf(orbitAngleY);
	float camY = targetPos_->y + offset_.y; // 高さは維持

	// カメラの位置を更新
	transform_.translate = { camX, camY, camZ };

	// カメラの向きをプレイヤーに向ける
	Vector3 lookDirection = *targetPos_ - transform_.translate;
	transform_.rotate.y = atan2f(lookDirection.x, lookDirection.z);
}

///-------------------------------------------/// 
///
///-------------------------------------------///
void Camera::FollowCollisionAvoidance() {
	// プレイヤーの回転を基にY軸回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateYMatrix(targetRot_->y);

	// オフセットを回転行列で変換し、適切な位置に調整
	Vector3 rotatedOffset = TransformVector(offset_, rotationMatrix);

	// 目標位置を算出
	Vector3 targetCameraPos = *targetPos_ + rotatedOffset;

	// 障害物の有無をチェック（レイキャストを使用予定）
	bool hitWall = false; // ここにレイキャストを実装する（仮）

	// 壁がある場合はターゲットに寄る
	if (hitWall) {
		transform_.translate = Lerp(transform_.translate, *targetPos_, followSpeed_);
	} else {
		transform_.translate = Lerp(transform_.translate, targetCameraPos, followSpeed_);
	}

	// カメラの向きをプレイヤーの回転に合わせる
	transform_.rotate = *targetRot_;
}
