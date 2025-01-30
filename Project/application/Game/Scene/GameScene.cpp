#include "GameScene.h"
// SceneManager
#include "Engine/Core/Mii.h"
#include "application/Manager/SceneManager.h"
#include <random>

///-------------------------------------------///
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	cardManager_.reset();
}

///-------------------------------------------///
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// モデルの読み込み
	const std::string& CardModel1 = "Card1";
	const std::string& CardModel2 = "Card2";
	const std::string& CardModel3 = "Card3";
	const std::string& CardModel4 = "Card4";

	Loader_->LoadModel(CardModel1);
	Loader_->LoadModel(CardModel2);
	Loader_->LoadModel(CardModel3);
	Loader_->LoadModel(CardModel4);

	Loader_->LoadModel("Particle");

	//std::list<std::string> cardModels;


	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {0.0f, 0.0f, -70.0f};
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = {0.0f, 0.0f, 0.0f};

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);

	std::vector<std::vector<int>> cardData;

	if (stage == StageNum::tutorial)
	{
		cardData = tutorial;
	}

	/// ===Model=== ///
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->Initialize(cardData,cameraManager_.get());


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
	globalVariables->SetValue("Cards", "CardGrid", cardData);
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

#ifdef USE_IMGUI

	const char* stageNames[] =
	{
		"tutorial",
		"card3x3_1",
		"card3x3_2",
		"card3x3_3",
		"card3x3_4",
		"card4x4_1",
		"card4x4_2",
		"card4x4_3",
		"card5x5_1",
		"card5x5_2",
		"card5x5_3",
		"card5x5_4",
	};

	if (ImGui::BeginCombo("Select Stage",stageNames[static_cast<int>(stage)])) {
		///
		if (ImGui::Selectable("tutorial")) {
			cardManager_->CardDataRefresh(tutorial);
			globalVariables->SetValue("Cards", "CardGrid", tutorial);
			stage = StageNum::tutorial;
		}
		///
		if (ImGui::Selectable("card3x3_1")) {
			cardManager_->CardDataRefresh(card3x3_1);
			globalVariables->SetValue("Cards", "CardGrid", card3x3_1);
			stage = StageNum::card3x3_1;
		}
		///
		if (ImGui::Selectable("card3x3_2")) {
			cardManager_->CardDataRefresh(card3x3_2);
			globalVariables->SetValue("Cards", "CardGrid", card3x3_2);
			stage = StageNum::card3x3_2;
		}
		///
		if (ImGui::Selectable("card3x3_3")) {
			cardManager_->CardDataRefresh(card3x3_3);
			globalVariables->SetValue("Cards", "CardGrid", card3x3_3);
			stage = StageNum::card3x3_3;
		}
		///
		if (ImGui::Selectable("card3x3_4")) {
			cardManager_->CardDataRefresh(card3x3_4);
			globalVariables->SetValue("Cards", "CardGrid", card3x3_4);
			stage = StageNum::card3x3_4;
		}
		///
		if (ImGui::Selectable("card4x4_1")) {
			cardManager_->CardDataRefresh(card4x4_1);
			globalVariables->SetValue("Cards", "CardGrid", card4x4_1);
			stage = StageNum::card4x4_1;
		}
		if (ImGui::Selectable("card4x4_2")) {
			cardManager_->CardDataRefresh(card4x4_2);
			globalVariables->SetValue("Cards", "CardGrid", card4x4_2);
			stage = StageNum::card4x4_2;
		}
		if (ImGui::Selectable("card4x4_3")) {
			cardManager_->CardDataRefresh(card4x4_3);
			globalVariables->SetValue("Cards", "CardGrid", card4x4_3);
			stage = StageNum::card4x4_3;
		}
		if (ImGui::Selectable("card5x5_1")) {
			cardManager_->CardDataRefresh(card5x5_1);
			globalVariables->SetValue("Cards", "CardGrid", card5x5_1);
			stage = StageNum::card5x5_1;
		}
		if (ImGui::Selectable("card5x5_2")) {
			cardManager_->CardDataRefresh(card5x5_2);
			globalVariables->SetValue("Cards", "CardGrid", card5x5_2);
			stage = StageNum::card5x5_2;
		}
		if (ImGui::Selectable("card5x5_3")) {
			cardManager_->CardDataRefresh(card5x5_3);
			globalVariables->SetValue("Cards", "CardGrid", card5x5_3);
			stage = StageNum::card5x5_3;
		}
		if (ImGui::Selectable("card5x5_4")) {
			cardManager_->CardDataRefresh(card5x5_4);
			globalVariables->SetValue("Cards", "CardGrid", card5x5_4);
			stage = StageNum::card5x5_4;
		}

		ImGui::EndCombo();
	}

#endif // USE_IMGUI

	mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

	cardManager_->Update(mousePosition_);

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	// camera_->SetRotate(cameraRotate_);
	cameraManager_->UpdateAllCameras();

	globalVariables->Update();

	 // すべてのカードが obtained ならシーンを変更
	if (cardManager_->AllCardsObtained()) {
		sceneManager_->ChangeScene("Clear");
	}
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

