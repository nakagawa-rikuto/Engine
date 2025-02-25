#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/CData.h"

///=====================================================/// 
/// カメラ
///=====================================================///
class Camera {
public:

	Camera() = default;
	~Camera() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

public:/// ===Getter=== ///
	// WorldMatrix
	const Matrix4x4& GetWorldMatrix()const;
	// ViewMatrix
	const Matrix4x4& GetViewMatrix()const;
	// ProjectionMatrix
	const Matrix4x4& GetProjectionMatrix()const;
	// ViewProjectionMatrix
	const Matrix4x4& GetViewProjectionMatrix()const;
	// Translate
	const Vector3& GetTranslate()const;
	// Rotate
	const Vector3& GetRotate()const;

public:/// ===Setter=== ///
	// Translate
	void SetTranslate(const Vector3& translate);
	// Rotate
	void SetRotate(const Vector3& rotate);
	// ForY
	void SetForY(const float& forY);
	// Aspect
	void SetAspectRatio(const float& aspect);
	// NearClip
	void SetNearClip(const float& nearClip);
	// FarClip
	void SetFarClip(const float& farClip);
	// 追従対象の座標を設定
	void SetTarget(Vector3* position, Vector3* rotation);
	// 追従オフセット
	void SetOffset(const Vector3& offset);
	// 追従速度を設定
	void SetFollowSpeed(float speed);

private:/// ===変数=== ///

	/// ===ビュー行列関連データ=== ///
	EulerTransform transform_;
	Matrix4x4 worldMatrix_;
	Matrix4x4 viewMatrix_;

	/// ===プロジェクション行列関連データ=== ///
	Matrix4x4 projectionMatrix_;
	float horizontalView_; // 水平方向視野角
	float aspect_;         // アスペクト比
	float nearClip_;       // ニアクリップ距離
	float farClip_;        // ファークリップ距離

	/// ===合成行列=== ///
	Matrix4x4 viewProjectionMatrix_;

	/// ===追従=== ///
	Vector3* targetPos_ = nullptr;  // 追従対象の座標ポインタ
	Vector3* targetRot_ = nullptr;  // 追従対象の回転ポインタ
	Vector3 offset_ = { 0.0f, 0.0f, -10.0f }; // カメラの初期オフセット
	float followSpeed_ = 0.1f;      // 追従速度

private:

	// 追従処理
	void FollowTarget();
};

