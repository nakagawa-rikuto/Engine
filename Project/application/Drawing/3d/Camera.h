#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/CData.h"

/// === カメラの種類を表す列挙型 === ///
enum class FollowCameraType {
	FixedOffset,        // 固定オフセット型
	Smoth,       // スムーズ追従型
	Orbiting,           // 回転型
	CollisionAvoidance, // 衝突回避型
};

///=====================================================/// 
/// カメラ
///=====================================================///
class Camera {
public:

	Camera() = default;
	~Camera();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImUpdate();

	/// <summary>
	/// FollowTypeの設定
	/// </summary>
	void SetFollowCamera(FollowCameraType type);

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
	const Quaternion& GetRotate()const;

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


public: /// ===追従(Setter)=== ///

	// 追従対象の座標を設定
	void SetTarget(Vector3* position, Quaternion* rotation);
	// 追従対象の速度
	void SetVelocity(const Vector3& velocity);
	// 追従オフセット
	void SetOffset(const Vector3& offset);
	// 回転型追従カメラのオフセット
	void SetOrbitingOffset(const Vector3& offset);
	// 追従速度を設定
	void SetFollowSpeed(float speed);
	// 回転補間速度を設定
	void SetLerpSpeed(float speed);
	// スティック
	void SetStick(const float& ValueX, const float& valueY);
	
private:/// ===変数=== ///

	/// ===ビュー行列関連データ=== ///
	QuaternionTransform transform_;
	QuaternionTransform addTransform_;
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
	// 追従対象の情報
	Vector3* targetPos_ = nullptr;
	Quaternion* targetRot_ = nullptr;
	// 追従オフセット
	Vector3 offset_ = { 0.0f, 0.0f, -20.0f };
	// 追従対象の速度
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	// 追従速度
	float followSpeed_ = 0.1f;
	// 回転補間速度
	float rotationLerpSpeed_ = 0.1f;
	// FollowCameraType
	FollowCameraType followType_ = FollowCameraType::FixedOffset;

	// 対象の周りを回るカメラの情報
	struct OrbitingInfo {
		Vector3 offset = {0.0f, 0.5f, -20.0f};
		float valueX = 0.0f;
		float valueY = 0.0f;
	};

	OrbitingInfo orbitingInfo_;

private: /// ===追従=== ///

	// カメラの種類に応じた更新処理
	void UpdateFollowCamera();
	// 固定オフセット型カメラの処理 
	void FollowFixedOffset();
	// スムーズなカメラ追従
	void FollowSmoth();
	// 回転可能型カメラの処理 
	void FollowOrbiting();
	// 衝突回避型カメラの処理 
	void FollowCollisionAvoidance();
};

