#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();

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
	player_->SetCamera(cameraManager_->GetActiveCamera().get());
	player_->Update();

	/// ===Enemy=== ///
	//enemy_->SetCamera(cameraManager_->GetActiveCamera().get());
	//enemy_->Update();

	/// ===Groundの更新=== ///
	ground_->SetCamera(cameraManager_->GetActiveCamera().get());
	ground_->Update();

	/// ===Cameraの更新=== ///
	cameraManager_->UpdateAllCameras();

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
	//enemy_->Draw();
	// Player
	player_->Draw();

	/// ===ISceneの描画=== ///
	IScene::Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}