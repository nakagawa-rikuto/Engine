#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// c++
#include <algorithm> 
// Service
#include "Engine/System/Service/Input.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();

	// camera
	FPSCamera_.reset();
	fixedPointCamera_.reset();
	// player
	player_.reset();
	// Ground
	ground_.reset();
	// SkyDome
	skyDome_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// FPSCamera
	FPSCamera_ = std::make_shared<Camera>();
	FPSCamera_->Initialize();
	// fixedPointCamera
	fixedPointCamera_ = std::make_shared<Camera>();
	fixedPointCamera_->Initialize();
	// カメラマネージャにカメラを追加
	cameraManager_->Add("FPSCamera", FPSCamera_);
	cameraManager_->Add("fixedPointCamera", fixedPointCamera_);
	// アクティブカメラの設定
	cameraManager_->SetActiveCamera("FPSCamera");

	// Player
	player_ = std::make_unique<Player>();
	player_->Initialize("MonsterBall");
	// Ground
	ground_ = std::make_unique<Ground>();
	ground_->Initialize("Ground");
	// SkyDome
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Initialize("skydome");
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");
	ImGui::End();

	player_->ImGuiUpdate();

#endif // USE_IMGUI

	 //===カメラの切り替え=== ///
	if (Input::TriggerKey(DIK_SPACE)) {
		if (SetFPSCamera_) {
			cameraManager_->SetActiveCamera("fixedPointCamera");
			SetFPSCamera_ = false;
		} else {
			cameraManager_->SetActiveCamera("FPSCamera");
			SetFPSCamera_ = true;
		}
	}

	// Ground
	ground_->Update(cameraManager_->GetActiveCamera().get());
	// SkyDome
	skyDome_->Update(cameraManager_->GetActiveCamera().get());

	// player
	player_->Update(cameraManager_->GetActiveCamera().get());

	// fixedPointCamera
	fixedPointCamera_->SetTranslate({0.0f, 5.0f, -40.0f});
	// 全てのカメラの更新
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	ground_->Draw();
	//skyDome_->Draw();
	// model
	player_->Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}