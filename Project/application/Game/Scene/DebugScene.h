#pragma once
/// ===Includ=== ///
#include "application/Game/Scene/IScene.h"
// Game
#include "application/2d/Sprite.h"
#include "application/3d/Model.h"
#include "application/3d/Camera.h"
#include "application/Particle/WindEmitter.h"

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

	/// ===クラス=== ///
	// スプライト
	std::unique_ptr<Sprite> sprite_;
	//Camera
	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Camera> camera2_;
	// モデル
	std::unique_ptr<Model> model_;

	/// ===変数=== ///
	// モデル
	Vector3 rotate = { 0.0f, 0.0f, 0.0f };
	Vector3 scale_ = { 1.0f, 1.0f, 1.0f }; 
	bool isRotate = false;  
	// sprite
	Vector2 size = { 100.0f, 100.0f };
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
	std::shared_ptr<WindEmitter> windParticle_;
};

