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

	auto it = cardDatas_.begin();

	std::advance(it, static_cast<int>(stage));

	/// ===Model=== ///
	cardManager_ = std::make_unique<CardManager>();
	cardManager_->Initialize(*it, cameraManager_.get());


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
	globalVariables->SetValue("Cards", "CardGrid", *it);
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

	RefreshCardData();

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

		CheckStarFlag();

		if (star2Flag)
		{
			sceneManager_->ChangeScene("Clear");
		}
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

void GameScene::RefreshCardData()
{
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

	if (ImGui::BeginCombo("Select Stage", stageNames[static_cast<int>(stage)])) {

		if (ImGui::Selectable("tutorial")) {
			stage = StageNum::tutorial;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_1")) {
			stage = StageNum::card3x3_1;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_2")) {
			stage = StageNum::card3x3_2;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_3")) {
			stage = StageNum::card3x3_3;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_4")) {
			stage = StageNum::card3x3_4;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_1")) {
			stage = StageNum::card4x4_1;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_2")) {
			stage = StageNum::card4x4_2;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_3")) {
			stage = StageNum::card4x4_3;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_1")) {
			stage = StageNum::card5x5_1;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_2")) {
			stage = StageNum::card5x5_2;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_3")) {
			stage = StageNum::card5x5_3;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_4")) {
			stage = StageNum::card5x5_4;
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(stage));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		ImGui::EndCombo();
	}
}

void GameScene::CheckStarFlag()
{
	Mission mission = stageMissions[static_cast<int>(stage)];


	if (mission.kStepCount > 0)
	{
		if (mission.kStepCount >= cardManager_->GetStepCount())
		{
			if (mission.kEraseAllCount1 > 0 && mission.kEraseAllCount1 <= cardManager_->GetAllObtainedCardCount())
			{
				star1Flag = true;
			}
		}
		else 
		{
			if (mission.kStepCount == 10 && cardManager_.get()->step10obtainedCount >= mission.kEraseAllCount1)
			{
				star1Flag = true;
			}

			if (mission.kStepCount == 15 && cardManager_.get()->step15obtainedCount >= mission.kEraseAllCount1)
			{
				star1Flag = true;
			}
		}
	}
	else
	{
		if (mission.kEraseAllCount1 > 0 && mission.kEraseAllCount1 <= cardManager_->GetAllObtainedCardCount())
		{
			star1Flag = true;
		}
	}

	if (mission.kEraseCardCount1 > 0)
	{
		if (mission.kEraseCardCount1 <= cardManager_->GetEraseCardCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}

	if (mission.kEraseCardCount2 > 0)
	{
		if (mission.kEraseCardCount2 <= cardManager_->GetEraseCardCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}
	

	if (mission.kMaxEraseCardCount1 > 0)
	{
		if (mission.kMaxEraseCardCount1 <= cardManager_->GetEraseCardMaxCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}

	if (mission.kEraseCount1 > 0)
	{
		if (mission.kEraseCount1 <= cardManager_->GetEraseCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}

	if (mission.kEraseCount2 > 0)
	{
		if (mission.kEraseCount2 <= cardManager_->GetEraseCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}

	if (mission.kEraseAllCount2 > 0)
	{
		if (mission.kEraseAllCount2 <= cardManager_->GetAllObtainedCardCount())
		{
			if (!star1Flag)
			{
				star1Flag = true;
			}
			else
			{
				star2Flag = true;
			}
		}
	}

}

