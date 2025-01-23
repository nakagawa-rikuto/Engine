#include "TitleScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"

///-------------------------------------------///
/// デストラクタ
///-------------------------------------------///
TitleScene::~TitleScene() {}

///-------------------------------------------///
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	Loader_->LoadModel("title");

	titleModel_ = std::make_unique<Model>();
	titleModel_->Initialize("title");

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {3.0f, 0.0f, -10.0f};
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = {0.0f, 0.0f, 0.0f};

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main2", camera_);
}

///-------------------------------------------///
/// 更新
///-------------------------------------------///
void TitleScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("TitleScene");

	ImGui::Begin("Camera");
	ImGui::DragFloat3("pos", &cameraPos_.x, 0.01f);
	ImGui::End();

	ImGui::End();
#endif // USE_IMGUI

	cameraManager_->SetActiveCamera("main2");
	camera_->SetTranslate(cameraPos_);
	cameraManager_->UpdateAllCameras();

	titleModel_->Update();
	titleModel_->SetCamera(cameraManager_->GetActiveCamera().get());
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void TitleScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画

	titleModel_->Draw();

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
