#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"
// Game
#include "application/Drawing/2d/Sprite.h"
#include "application/Drawing/3d/Object3d.h"
#include "application/Game/Camera/camera.h"
// Line
#include "application/Drawing/3d/Line.h"
// Math
#include "Math/sMath.h"

#include <numbers>

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class DebugScene : public IScene {
public:/// ===メンバ関数=== ///

	DebugScene() = default;
	~DebugScene();

	// 初期化
	void Initialize() override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;

private:/// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>

	/// ===ImGui用フラグ=== ///
#pragma region ImGui
	struct Setting {
		bool Sprite;
		bool Model;
		bool Particle1;
		bool Particle2;
		bool Particle3;
	};
	struct Display {
		bool Sprite;
		bool Model;
		bool Particle1;
		bool Particle2;
		bool Particle3;
	};
	struct Imgui {
		bool Sprite;
		bool Model;
		bool Particle1;
		bool Particle2;
		bool Particle3;
	};
	struct LightTypeInfo {
		bool Lambert;
		bool HalfLambert;
		bool PointLight;
		bool SpotLight;
		bool None;
	};
	Setting isSetting_ = { false };
	Display isDisplay_ = { false };
	Imgui isImgui_ = { false };
	LightTypeInfo lightType_ = { false };
#pragma endregion

	/// ===クラス=== ///
#pragma region クラス
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	//Camera
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Camera> camera2_;
	// モデル
	std::unique_ptr<Object3d> model_;
	std::unique_ptr<Object3d> model2_;
	std::unique_ptr<Object3d> animationModel_;
	// モデルライト
	std::unique_ptr<Object3d> modelLight_;
	// Line
	std::unique_ptr<Line> line_;
#pragma endregion

	/// ===変数=== ///
#pragma region 変数
	// モデル
	Vector3 modelTranslate_ = { 0.0f, -1.3f, 0.0f };
	Quaternion modelRotate_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	Vector3 modelScale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 modelColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool isRotate = false;
	// sprite
	Vector2 spriteTranslate_ = { 0.0f, 0.0f };
	float spriteRotate_ = 0.0f;
	Vector2 spriteSize_ = { 100.0f, 100.0f };
	Vector4 spriteColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ライト
	LightInfo light_ = {
		{ 48.5f } ,
		{ { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f, 0.0f } , 1.0f },
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 2.0f, 0.0f } , 1.0f, 10.0f, 1.0f},
		{{ 1.0f, 1.0f, 1.0f, 1.0f } , { 2.0f, 1.25f, 0.0f } , 4.0f, Normalize(Vector3{ -1.0f, -1.0f, 0.0f }), 7.0f, 2.0f, std::cos(std::numbers::pi_v<float> / 3.0f)}
	};
	// カメラ
	Vector3 cameraPos = { 0.0f, 0.0f, -13.0f };
	Quaternion cameraRotate = { 0.0f, 0.0f, 0.0f, 1.0f };
	bool SetCamera = false;
	// Audio
	bool playAudio = false;
	float volume = 1.0f;
	float pitch = 1.0f;
	// マウス
	bool PushLeft_ = false;
	bool TriggerRight_ = false;
	Vector2 mousePosition_ = { 0.0f, 0.0f };
	// Particle
	bool start_ = false;
	Vector3 particleTranslate_ = { 0.0f, 0.0f, 0.0f };
	// OffScreen
	bool isGrayscale = false;
	// Line
	struct LineInfo {
		Vector3 startPos = { 0.0f, 0.0f, 0.0f };
		Vector3 endPos = { 10.0f, 0.0f, 0.0f };
		Vector4 color = { 1.0f, 0.0f, 0.0f, 1.0f };
	};
	LineInfo lineInfo_;
	
#pragma endregion
};