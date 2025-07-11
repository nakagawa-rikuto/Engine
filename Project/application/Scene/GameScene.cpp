#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/CameraService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	skyBox_.reset();
	debugAnimationModel_.reset();

	// ISceneのデストラクタ
	IScene::~IScene();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetFollowCamera(FollowCameraType::Orbiting);
	camera_->SetOffset({ 0.0f, 2.0f, -20.0f });
	camera_->SetOrbitingOffset({ 0.0f, 2.0f, -20.0f });
	camera_->SetFollowSpeed(1.0f);
	// カメラの追加
	CameraService::Add("Game", camera_);
	CameraService::SetActiveCamera("Game");

	// Animation
	debugAnimationModel_ = std::make_unique<DebugAnimationModel>();
	debugAnimationModel_->Initialize();

	// SkyBox
	skyBox_ = std::make_unique<SkyBox>();
	skyBox_->Initialize("skyBox", LightType::None);
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

	// カメラ
	camera_->SetTarget(&debugAnimationModel_->GetTranslate(), &debugAnimationModel_->GetRotate());

	// 更新
	debugAnimationModel_->Update();
	skyBox_->Update();

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

	// SkyBoxの描画
	skyBox_->Draw();
	// Animationの描画
	debugAnimationModel_->Draw();

	/// ===ISceneの描画=== ///
	IScene::Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}