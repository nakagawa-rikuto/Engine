#include "GameScene.h"
// SceneManager
#include "Engine/Core/Mii.h"
#include "application/Manager/SceneManager.h"
#include <random>

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
	const std::string& CardModel = "Card1";
	Loader_->LoadModel(CardModel);
	Loader_->LoadModel("Particle");

	std::list<std::string> cardModels;

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {5.0f, -2.5f, -70.0f};
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = {0.0f, 0.0f, 0.0f};

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);

	/// ===Model=== ///
	// const int gridSize = 5;                       // グリッドのサイズ
	
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->Initialize(cardData3x3, cardModels, cameraManager_.get());


	// GlobalVariablesの取得
	globalVariables = GlobalVariables::GetInstance();

	// カードデータの登録
	const char* groupNameCards = "Cards";
	globalVariables->CreateGroup(groupNameCards);

	// デバッグ表示用に GlobalVariables にデータを登録
	std::vector<int32_t> cardGrid;
	for (int y = 0; y < cardManager_->GetRows(); ++y) {
		for (int x = 0; x < cardManager_->GetCols(); ++x) {
			cardGrid.push_back(cardManager_->GetCards()[y][x]->GetCardType());
		}
	}
	globalVariables->SetValue("Cards", "CardGrid", cardData3x3);
}

///-------------------------------------------///
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバッグ用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");

	// ペアが揃っていない値を取得
	std::vector<int32_t> missingPairs = globalVariables->CheckMissingPairs("Cards", "CardGrid");

	// 不足しているペアを表示
	if (!missingPairs.empty()) {
		ImGui::Text("Missing pairs detected:");
		for (int value : missingPairs) {
			ImGui::Text("Value %d is missing a pair.", value);
		}
	} else {
		ImGui::Text("All pairs are complete.");
	}

	ImGui::End();

	ImGui::Begin("Camera");
	ImGui::DragFloat3("pos", &cameraPos_.x, 0.01f);
	ImGui::End();

#endif // USE_IMGUI

#ifdef USE_IMGUI
	ImGui::Begin("State");

	ImGui::End();

#endif // USE_IMGUI

#ifdef USE_IMGUI
	ImGui::Begin("selectIndex");
	ImGui::Text("Index Z = %d\nIndex X = %d", zIndex, xIndex);
	ImGui::End();

#endif // USE_IMGUI

	mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

	cardManager_->Update(mousePosition_);

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	// camera_->SetRotate(cameraRotate_);
	cameraManager_->UpdateAllCameras();

	globalVariables->Update();
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画

	cardManager_->Darw();

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

