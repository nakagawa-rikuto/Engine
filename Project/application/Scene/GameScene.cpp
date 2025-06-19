#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
#include "Engine/System/Service/ServiceCamera.h"
#include "Engine/System/Service/ServiceParticle.h"

// Particle
#include "Engine/Graphics/Particle/Derivative/ConfettiParticle.h"
#include "Engine/Graphics/Particle/Derivative/ExplosionParticle.h"
#include "Engine/Graphics/Particle/Derivative/WindParticle.h"
#include "Engine/Graphics/Particle/Derivative/HitEffectParticle.h"
#include "Engine/Graphics/Particle/Derivative/RingParticle.h"
#include "Engine/Graphics/Particle/Derivative/CylinderParticle.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();

	// Colliderのリセット
	colliderManager_->Reset();

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
	camera_->SetTranslate(cameraInfo_.translate);
	camera_->SetRotate(cameraInfo_.rotate);
	// Managerに追加,アクティブに
	ServiceCamera::Add("Game", camera_);
	ServiceCamera::SetActiveCamera("Game");

	/// ===Player=== ///
	player_ = std::make_unique<Player>();
	player_->Init(ServiceCamera::GetActiveCamera().get());

	/// ===Enemy=== ///
	enemy_ = std::make_unique<Enemy>();
	enemy_->Init(ServiceCamera::GetActiveCamera().get(), player_.get());

	/// ===Ground=== ///
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	/// ===ColliderManager=== ///
	colliderManager_ = std::make_unique<ColliderManager>();
	colliderManager_->Initialize();
	// Colliderの追加
	colliderManager_->AddCollider(player_.get());
	colliderManager_->AddCollider(enemy_.get());

	// Particleの追加
	ServiceParticle::AddParticle("Confetti", std::make_unique<ConfettiParticle>());
	ServiceParticle::AddParticle("Explosion", std::make_unique<ExplosionParticle>());
	ServiceParticle::AddParticle("Wind", std::make_unique<WindParticle>());
	ServiceParticle::AddParticle("Ring", std::make_unique<RingParticle>());
	ServiceParticle::AddParticle("HitEffect", std::make_unique<HitEffectParticle>());
	ServiceParticle::AddParticle("Cylinder", std::make_unique<CylinderParticle>());
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
	ImGui::Begin("Camera");
	ImGui::DragFloat3("Translate", &cameraInfo_.translate.x, 0.01f);
	ImGui::DragFloat4("Rotate", &cameraInfo_.rotate.x, 0.001f);
	ImGui::End();
	// Player
	player_->UpdateImGui();

	// Enemy
	//enemy_->UpdateImGui();

#endif // USE_IMGUI

	/// ===Playerの更新=== ///
	player_->SetCamera(ServiceCamera::GetActiveCamera().get());
	player_->Update();

	/// ===Enemy=== ///
	enemy_->SetCamera(ServiceCamera::GetActiveCamera().get());
	enemy_->Update();

	/// ===Groundの更新=== ///
	ground_->SetCamera(ServiceCamera::GetActiveCamera().get());
	ground_->Update();

	/// ===ColliderManager=== ///
	colliderManager_->CheckAllCollisions();

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