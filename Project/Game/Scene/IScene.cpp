#include "IScene.h"

#include "Engine/Core/Mii.h"

void IScene::Initialize() {

	/// ===デフォルトカメラの生成=== ///
	defaultCamera_ = std::make_unique<Camera>();
	defaultCamera_->Initialize();
	defaultCamera_->SetTranslate({ 0.0f, 0.0f, -10.0f });
	defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });

	/// ===カメラマネージャの生成=== ///
	cameraManager_ = std::make_unique<CameraManager>();
	cameraManager_->Initialize();

	/// ===Audio=== ///
	audio_ = std::make_unique<Audio>();
	audio_->Initialze(Mii::GetAudioManager());

	/// ===Loadの生成=== ///
	Loader_ = std::make_unique<AssetLoader>();
	Loader_->Inititalze(Mii::GetTextureManager(), Mii::GetModelManager(), Mii::GetAudioManager());
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
