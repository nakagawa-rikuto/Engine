#include "IScene.h"


void IScene::Initialize() {
	/// ===Input=== ///
	input_ = Mii::GetInput();

	/// ===デフォルトカメラの生成=== ///
	defaultCamera_ = std::make_unique<Camera>();
	defaultCamera_->Initialize();
	defaultCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	/// ===カメラマネージャの生成=== ///
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
