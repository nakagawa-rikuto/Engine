#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/ColliderService.h"
#include "Engine/System/Service/InputService.h"
// Particle
#include "Engine/Graphics/Particle/Derivative/ConfettiParticle.h"
#include "Engine/Graphics/Particle/Derivative/ExplosionParticle.h"
#include "Engine/Graphics/Particle/Derivative/WindParticle.h"
#include "Engine/Graphics/Particle/Derivative/HitEffectParticle.h"
#include "Engine/Graphics/Particle/Derivative/RingParticle.h"
#include "Engine/Graphics/Particle/Derivative/CylinderParticle.h"
#include "Math/SMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();
	// Colliderのリセット
	ColliderService::Reset();
	// Camera
	camera_.reset();
	// Player
	player_.reset();
	// Ground
	ground_.reset();
	// Enemy
	enemy_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===Camera=== ///
	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetRotate(cameraInfo_.rotate);
	camera_->SetFollowCamera(FollowCameraType::TopDown);
	camera_->SetOffset({ 0.0f, 70.0f, -60.0f });
	camera_->SetFollowSpeed(0.1f);
	// Managerに追加,アクティブに
	CameraService::Add("Game", camera_);
	CameraService::SetActiveCamera("Game");

	/// ===Player=== ///
	player_ = std::make_unique<Player>();
	player_->Initialize();

	/// ===Enemy=== ///
	enemy_ = std::make_unique<CloseRangeEnemy>();
	enemy_->Initialize();
	enemy_->SetPlayer(player_.get()); // Playerを設定

	/// ===Ground=== ///
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	// Colliderの追加
	ColliderService::AddCollider(player_.get());
	ColliderService::AddCollider(enemy_.get());

	// Particleの追加
	ParticleService::AddParticle("Confetti", std::make_unique<ConfettiParticle>());
	ParticleService::AddParticle("Explosion", std::make_unique<ExplosionParticle>());
	ParticleService::AddParticle("Wind", std::make_unique<WindParticle>());
	ParticleService::AddParticle("Ring", std::make_unique<RingParticle>());
	ParticleService::AddParticle("HitEffect", std::make_unique<HitEffectParticle>());
	ParticleService::AddParticle("Cylinder", std::make_unique<CylinderParticle>());
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");
	ImGui::End();

	// Camera
	camera_->UpdateImGui();

	// Player
	player_->UpdateImGui();

	// Enemy
	enemy_->UpdateImGui();

#endif // USE_IMGUI

	/// ===DebugCamera=== ///
	camera_->DebugUpdate();

	/// ===Playerの更新=== ///
	player_->Update();

	/// ===Enemy=== ///
	enemy_->Update();

	/// ===Groundの更新=== ///
	ground_->Update();

	/// ===ColliderManager=== ///

	/// ===ISceneの更新=== ///
	IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
	
#pragma endregion

#pragma region モデル描画

	// Ground
	ground_->Draw();
	// Enemy
	enemy_->Draw();
	// Player
	player_->Draw();

	/// ===ISceneの描画=== ///
	IScene::Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}