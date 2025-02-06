#pragma once
/// ===Includ=== ///
#include "application/Scene/IScene.h"
// Game
#include "application/Drawing/2d/Sprite.h"
#include "application/Drawing/3d/Model.h"
#include "application/Drawing/3d/Camera.h"
#include "application/Drawing/Particle/WindEmitter.h"
#include "application/Drawing/Particle/ExplosionEmitter.h"
#include "application/Drawing/Particle/ConfettiEmitter.h"

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
	Setting isSetting_ = {false};
	Display isDisplay_ = {false};
	Imgui isImgui_ = {false};

	/// ===クラス=== ///
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	//Camera
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Camera> camera2_;
	// モデル
	std::unique_ptr<Model> model_;
	std::unique_ptr<Model> model2_;
	// モデルライト
	std::unique_ptr<Model> modelLight_;
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
	LightInfo light_ = { 48.5f };
	// Directional
	DirectionalLightInfo directional_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 1.0f, 0.0f } , 1.0f };
	// point
	PointLightInfo point_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 0.0f, 2.0f, 0.0f } , 1.0f, 10.0f, 1.0f };
	// spotLight
	SpotLightInfo spot_ = { { 1.0f, 1.0f, 1.0f, 1.0f } , { 2.0f, 1.25f, 0.0f } , 4.0f, Normalize({ -1.0f, -1.0f, 0.0f }) , 7.0f, 2.0f, std::cos(std::numbers::pi_v<float> / 3.0f) };
	// カメラ
	Vector3 cameraPos = { 0.0f, 2.0f, -13.0f };  
	Vector3 cameraRotate = { 0.1f, 0.0f, 0.0f };
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