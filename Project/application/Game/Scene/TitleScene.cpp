#include "TitleScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"

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

	const std::string& titleTexture = "./Resource/title.png";
	Loader_->LoadTexture(titleTexture);

	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize(titleTexture);

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {3.0f, -0.5f, -10.0f};
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

	ImGui::Text("Space To GameScene");

	ImGui::Separator();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("pos", &cameraPos_.x, 0.01f);

	ImGui::End();

	ImGui::End();
#endif // USE_IMGUI

	cameraManager_->SetActiveCamera("main2");
	camera_->SetTranslate(cameraPos_);
	cameraManager_->UpdateAllCameras();

	titleSprite_->Update();

	if (Mii::TriggerKey(DIK_SPACE) || Mii::TriggerMouse(MouseButtonType::Left)) {
		sceneManager_->ChangeScene("Select");
	}
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void TitleScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画

	titleSprite_->Draw();

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
