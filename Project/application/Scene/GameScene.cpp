#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"


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
	cameraManager_->Add("Game", camera_);
	cameraManager_->SetActiveCamera("Game");
	cameraManager_->UpdateAllCameras();

	/// ===Player=== ///
	player_ = std::make_unique<Player>();
	player_->Init(cameraManager_->GetActiveCamera().get());

	/// ===Enemy=== ///
	enemy_ = std::make_unique<Enemy>();
	enemy_->Init(cameraManager_->GetActiveCamera().get(), player_.get());

	/// ===Ground=== ///
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();

	/// ===ColliderManager=== ///
	colliderManager_ = std::make_unique<ColliderManager>();
	// Colliderの追加
	colliderManager_->AddCollider(player_.get());
	colliderManager_->AddCollider(enemy_.get());
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
	player_->SetCamera(cameraManager_->GetActiveCamera().get());
	player_->Update();

	/// ===Enemy=== ///
	enemy_->SetCamera(cameraManager_->GetActiveCamera().get());
	enemy_->Update();

	/// ===Groundの更新=== ///
	ground_->SetCamera(cameraManager_->GetActiveCamera().get());
	ground_->Update();

	/// ===Cameraの更新=== ///
	cameraManager_->UpdateAllCameras();

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