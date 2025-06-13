#include "IScene.h"
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/Graphics/3d/Line/LineObject3D.h"
#include "Engine/System/Service/ServiceCamera.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
IScene::~IScene() {
	defaultCamera_.reset();
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

	// デフォルトカメラの設定
	ServiceCamera::Add("Default", defaultCamera_);
	ServiceCamera::SetActiveCamera("Default");
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void IScene::Update() {
	ServiceLocator::GetLineObject3D()->SetCamera(ServiceCamera::GetActiveCamera().get());
	ServiceLocator::GetLineObject3D()->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void IScene::Draw() {
	ServiceLocator::GetLineObject3D()->Draw();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
