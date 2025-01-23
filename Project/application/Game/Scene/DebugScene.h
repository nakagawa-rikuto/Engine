#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
// Game
#include "application/2d/Sprite.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"
#include "application/Particle/WindEmitter.h"
#include "application/Particle/ExplosionEmitter.h"
#include "application/Particle/ConfettiEmitter.h"

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
	Setting isSetting_ = { false };
	Display isDisplay_ = { false };
	Imgui isImgui_ = { false };

	/// ===クラス=== ///
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	//Camera
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Camera> camera2_;
	// モデル
	std::unique_ptr<Model> model_;
	// Particle
	std::shared_ptr<WindEmitter> windParticle_;
	std::shared_ptr<ExplosionEmitter> explosionParticle_;
	std::shared_ptr<ConfettiEmitter> confettiParticle_;

	/// ===変数=== ///
	// モデル
	Vector3 modelTranslate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelRotate_ = { 0.0f, 0.0f, 0.0f };
	Vector3 modelScale_ = { 1.0f, 1.0f, 1.0f };
	Vector4 modelColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	bool isRotate = false;
	// sprite
	Vector2 spriteTranslate_ = { 0.0f, 0.0f };
	float spriteRotate_ = 0.0f;
	Vector2 spriteSize_ = { 100.0f, 100.0f };
	Vector4 spriteColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ライト
	Vector3 lightDirection_ = { 0.0f, 1.0f, 0.0f };
	float lightIntensity_ = 1.0f;
	Vector4 lightColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	float lightShininess_ = 48.5f;
	// カメラ
	Vector3 cameraPos = { 0.0f, 0.0f, -10.0f };
	Vector3 cameraRotate = { 0.0f, 0.0f, 0.0f };
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
};

