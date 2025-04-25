#include "IScene.h"

IScene::~IScene() {
	defaultCamera_.reset();
	cameraManager_.reset();
	particleManager_.reset();
}

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
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
