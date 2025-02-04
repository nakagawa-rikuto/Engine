#pragma once
/// ===Include=== ///
#include "Engine/Core/CData.h"

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
	void Info();

public:/// ===追加=== ///
	// 2D用の初期化
	void InitializeFor2D();
	// 切り替え
	void SwitchTo2DMode();
	void SwitchTo3DMode();

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
	// Scale
	void SetScale(const Vector3& scale);
	// ForY
	void SetForY(const float& forY);
	// Aspect
	void SetAspectRatio(const float& aspect);
	// NearClip
	void SetNearClip(const float& nearClip);
	// FarClip
	void SetFarClip(const float& farClip);

private:/// ===変数=== ///

	/// ===ビュー行列関連データ=== ///
	WorldTransform transform_;
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

private:/// ===追加=== ///
	bool is2DMode_ = false;
};

