#include "IScene.h"
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/Graphics/3d/Line/LineObject3D.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
IScene::~IScene() {
	defaultCamera_.reset();
	cameraManager_.reset();
	particleManager_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void IScene::Initialize() {

	/// ===デフォルトカメラの生成=== ///
	defaultCamera_ = std::make_shared<Camera>();
	defaultCamera_->Initialize();
	defaultCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	/// ===カメラマネージャの生成=== ///
	cameraManager_ = std::make_unique<CameraManager>();
	// デフォルトカメラの設定
	cameraManager_->Add("Default", defaultCamera_);
	cameraManager_->SetActiveCamera("Default");

	/// ===Particlemanagerの生成=== ///
	particleManager_ = std::make_unique<ParticleManager>();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void IScene::Update() {
	// ParticleManagerの更新
	particleManager_->SetCamera(cameraManager_->GetActiveCamera().get());
	particleManager_->Update();

	// Line更新
	ServiceLocator::GetLineObject3D()->SetCamera(cameraManager_->GetActiveCamera().get());
	ServiceLocator::GetLineObject3D()->Update();

	// カメラの更新
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void IScene::Draw() {
	// ParticleManagerの描画
	particleManager_->Draw();

	// Lineの描画
	ServiceLocator::GetLineObject3D()->Draw();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
