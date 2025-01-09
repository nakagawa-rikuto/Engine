#include "GameScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// モデルの読み込み
	const std::string& CardModel = "Card";
	Loader_->LoadModel(CardModel);

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = { 0.0f, 0.0f, -50.0f };
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = { 0.0f, 0.0f, 0.0f };

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);


	/// ===Model=== ///
	const int gridSize = 5;  // グリッドのサイズ
	const float spacing = 10.0f;  // モデル間の間隔
	const Vector3 basePosition(0.0f, 0.0f, 0.0f); // 基準となる位置

	for (size_t z = 0; z < gridSize; ++z) {
		for (size_t x = 0; x < gridSize; ++x) {

			auto card = std::make_shared<Model>();

			Vector3 position(
				basePosition.x + x * spacing,
				basePosition.y,
				basePosition.z + z * spacing
			);

			card->Initialize(CardModel);
			card->SetPosition(position);
			card->SetCamera(cameraManager_->GetActiveCamera().get());
			cards_.push_back(card);
		}
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");
	ImGui::End();
#endif // USE_IMGUi

#ifdef USE_IMGUI
	ImGui::Begin("Camera");
	ImGui::DragFloat3("CameraTranslate", &cameraPos_.x, 0.01f);
	ImGui::DragFloat3("CameraRotate", &cameraRotate_.x, 0.01f);
	ImGui::DragFloat3("CameraScale", &cameraScale_.x, 0.01f);
	ImGui::End();

#endif // USE_IMGUI

	for (const auto& card : cards_) {
		card->SetCamera(cameraManager_->GetActiveCamera().get());
		card->Update();
	}

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	camera_->SetScale(cameraScale_);
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	for (const auto& card : cards_) {
		card->Draw();
	}
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}