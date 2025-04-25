#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();

	// Player
	player_.reset();
	// camera
	camera_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===Playerの初期化=== ///
	player_ = std::make_unique<Player>();
	player_->Initialize();

	/// ===カメラの初期化=== ///
	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate({ 0.0f, 10.0f, 0.0f });
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	// カメラマネージャにカメラを追加
	cameraManager_->Add("main", camera_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");



	ImGui::End();
#endif // USE_IMGUI

	/// ===Playerの更新=== ///
	player_->Update();
	/// ===カメラの更新=== ///
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	// Playerの描画
	player_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}