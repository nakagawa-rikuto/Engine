#include "GameScene.h"
// SceneManager
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
	const std::string& CardModel = "Card";
	Loader_->LoadModel(CardModel);

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {0.0f, 0.0f, -70.0f};
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = {0.0f, 0.0f, 0.0f};

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);

	/// ===Model=== ///
	const int gridSize = 5;                       // グリッドのサイズ
	const float spacing = 5.0f;                   // モデル間の間隔
	const Vector3 basePosition(0.0f, 0.0f, 0.0f); // 基準となる位置

	for (size_t z = 0; z < gridSize; ++z) {
		for (size_t x = 0; x < gridSize; ++x) {

			auto card = std::make_shared<Model>();

			Vector3 position(basePosition.x + x * spacing, basePosition.y + z * spacing, basePosition.z);

			card->Initialize(CardModel);
			card->SetPosition(position);
			card->SetCamera(cameraManager_->GetActiveCamera().get());
			cards_.push_back(card);
		}
	}

	// GlobalVariablesの取得
	globalVariables = GlobalVariables::GetInstance();

	// カードデータの登録
	const char* groupNameCards = "Cards";
	globalVariables->CreateGroup(groupNameCards);

	// 1〜8の値を2つずつ用意
	cardAnswers.clear();
	for (int i = 1; i <= 8; ++i) {
		cardAnswers.push_back(i);
		cardAnswers.push_back(i);
	}

	// シャッフル
	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(cardAnswers.begin(), cardAnswers.end(), gen);

	// ベクター全体を登録
	globalVariables->SetValue(groupNameCards, "CardGrid", cardAnswers);
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
#endif // USE_IMGUI

	// 他の更新処理
	for (const auto& card : cards_) {
		card->SetCamera(cameraManager_->GetActiveCamera().get());
		card->Update();
	}

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
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
	for (const auto& card : cards_) {
		card->Draw();
	}
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}