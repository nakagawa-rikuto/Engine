#include "Camera.h"

#include "Math/sMath.h"
#include "Math/EasingMath.h"
#include "Engine/System/Service/Getter.h"

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
const Vector3& Camera::GetTranslate() const { return transform_.translate; }
// Rotate
const Quaternion& Camera::GetRotate() const { return transform_.rotate; }

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void Camera::SetTranslate(const Vector3& translate) { addTransform_.translate = translate; }
// Rotate
void Camera::SetRotate(const Vector3& rotate) { addTransform_.rotate = {rotate.x, rotate.y, rotate.z, 1.0f}; }
// ForY
void Camera::SetForY(const float& forY) { horizontalView_ = forY; }
// AspectRatio
void Camera::SetAspectRatio(const float& aspect) { aspect_ = aspect; }
// NearClip
void Camera::SetNearClip(const float& nearClip) { nearClip_ = nearClip; }
// FarClip
void Camera::SetFarClip(const float& farClip) { farClip_ = farClip; }
// 追従対象の座標を設定
void Camera::SetTarget(Vector3* position, Quaternion* rotation) {
	targetPos_ = position;
	targetRot_ = rotation;
}
// 追従のオフセット
void Camera::SetOffset(const Vector3& offset) { offset_ = offset; }
void Camera::SetOrbitingOffset(const Vector3& offset) { OrbitingOffset_ = offset; }
// 追従速度を設定
void Camera::SetFollowSpeed(float speed) { followSpeed_ = speed; }
// 回転補間速度
void Camera::SetLerpSpeed(float speed) { rotationLerpSpeed_ = speed; }
// 回転の量
void Camera::SetStick(const Vector2& stickValue) { stickValue_ = stickValue; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Camera::Initialize() {
	transform_ = { {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.f} };
	addTransform_ = { {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.f} };
	horizontalView_ = 0.45f;
	aspect_ = static_cast<float>(Getter::GetWindowWidth()) / static_cast<float>(Getter::GetWindowHeight());
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
// 
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
// 回転軸がY座標だけの追従処理
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
// 回転軸がXとY座標の追従処理
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
// 自分の周りをまわるカメラの追従処理
void Camera::FollowOrbiting() {
	// クォータニオンで回転を管理
	Quaternion rotationDelta = IdentityQuaternion();

	// 右スティックのX・Y軸の値を取得 (-32768 ～ 32767)
	float rightStickX = stickValue_.x; // Yaw（左右回転）
	float rightStickY = stickValue_.y; // Pitch（上下回転）

	// デッドゾーン処理（スティックがわずかに傾いたときの無効化）
	const float DEADZONE = 0.2f;
	if (fabs(rightStickX) < DEADZONE) rightStickX = 0.0f;
	if (fabs(rightStickY) < DEADZONE) rightStickY = 0.0f;

	// スティックの入力を回転量に変換
	float deltaYaw = rightStickX * 0.05f;  // 感度調整
	float deltaPitch = rightStickY * 0.05f;

	// クォータニオンを用いた回転計算
	Quaternion yawRotation = MakeRotateAxisAngle(
		Vector3(0, 1, 0), deltaYaw);
	Quaternion pitchRotation = MakeRotateAxisAngle(
		RotateVector(Vector3(1, 0, 0), yawRotation * transform_.rotate), deltaPitch);

	// 回転の補間
	rotationDelta = pitchRotation * yawRotation;


	// 累積回転を更新
	transform_.rotate = rotationDelta * transform_.rotate;

	offset_ = OrbitingOffset_;

	// 回転を適用
	offset_ = RotateVector(offset_, transform_.rotate);
	transform_.translate = offset_ + *targetPos_;

	transform_.rotate = Normalize(transform_.rotate); // クォータニオンを正規化して数値誤差を防ぐ
}
// 障害物を避ける追従処理
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
