#pragma once
/// ===Include=== ///
// c++
#include <optional>
// application
#include "application/Drawing/3d/Model.h"

/// ===前方宣言=== ///
class Camera;

///-------------------------------------------/// 
/// プレイヤー
///-------------------------------------------///
class Player {
public:

	Player() = default;
	~Player();

	void Initialize(const std::string& modelName, Camera* camera);
	void Update();
	void Draw();

	void ImGuiUpdate();

private:

	// モデル
	std::unique_ptr<Model> model_;

	// Camera
	Camera* camera_ = nullptr;

	/// ===変数=== ///
	Vector3 translate_ = { 0.0f, 0.0f, 0.0f };
	Quaternion rotate_ = { 0.0f, 0.0f, 0.0f, 0.0f };
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 振る舞い（Behavior）
	enum class Behavior {
		kRoot, // 通常状態
		kMove, // 移動
		kBoost // ブースト
	};
	Behavior behavior_ = Behavior::kRoot;
	// 次の振る舞いリクエスト
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	/// ===Energy情報=== ///
	struct EnergyInfo {
		float rest = 0.0f;        // 現在のエネルギー量
		float max = 10000.0f; // エネルギーの最大値
		float drain = 1.0f;   // エネルギーの消費量
		float regen = 10.0f;  // エネルギー回復量
	};
	/// ===Move情報=== ///
	struct MoveInfo {
		float speed = 3.0f;          // 移動速度
		float rotationSpeed = 0.05f; // 視点回転速度
		float maxPitch = 1.2f;       // 上下回転速度
	};
	/// ===Boost情報=== ///
	struct BoostInfo {
		float speed = 0.0f;             // 現在のスビード	
		float maxSpeed = 20.0f;          // 最大スピード
		float rotationSpeed = 0.005f;     // 視点回転速度
		float maxPitch = 1.2f;           // 上下回転速度
		float coolDown = 0.0f;           // 現在のクールタイム
		float maxCoolDown = 3.0f;        // クールタイムの最大値
		float deccelerationRate = 10.0f; // 減速率
	};
	/// ===Jump情報=== ///
	struct JumpInfo {
		float verticalSpeed = 3.0f;     // 上昇・下降速度
		float gravity = -9.8f;          // 重力加速度
		float verticalVelocity = 0.0f;  // 現在のY軸速度
		float groundLevel = 0.0f;       // 地面のY軸座標
	};
	/// ===Camera情報=== ///
	struct CameraInfo {
		Vector3 offset = { 0.0f, 0.0f, 0.0f };        // 現在のオフセット
		Vector3 moveOffset = { 0.0f, 0.5f, -20.0f };  // 通常のオフセット
		Vector3 boostOffset = { 2.0f, 0.8f, -10.0f }; // ブースト時のオフセット
		Vector3 translate = { 0.0f, 0.0f, 0.0f };     // カメラの位置
		Quaternion rotate = { 0.0f, 0.0f, 0.0f, 0.0f };        // カメラの位置
		float lerpSpeed = 4.0f;                       // 補間速度
		float blend = 0.0f;                           // 補間係数
	};

	EnergyInfo energyInfo_;
	MoveInfo moveInfo_;
	BoostInfo boostInfo_;
	JumpInfo jumpInfo_;
	CameraInfo cameraInfo_;

	const float deltaTime_ = 1.0f / 60.0f;

private:
	// 通常
	void InitializeRoot(); //初期化
	void UpdateRoot(); // 更新
	// 移動
	void InitializeMove(); //初期化
	void UpdateMove(); // 更新
	// ブースト
	void InitializeBoost(); //初期化
	void UpdateBoost(); // 更新
	// カメラ
	void UpdateCamera();
	// 上昇
	void UpdateAir();
};

