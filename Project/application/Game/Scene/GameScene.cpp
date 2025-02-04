#include "GameScene.h"
// SceneManager
#include "Engine/Core/Mii.h"
#include "application/Manager/SceneManager.h"
#include <random>

///-------------------------------------------///
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() { cardManager_.reset(); }

///-------------------------------------------///
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===読み込み=== ///
	// モデルの読み込み
	const std::string& CardModel1 = "Card1";
	const std::string& CardModel2 = "Card2";
	const std::string& CardModel3 = "Card3";
	const std::string& CardModel4 = "Card4";
	Loader_->LoadModel(CardModel1);
	Loader_->LoadModel(CardModel2);
	Loader_->LoadModel(CardModel3);
	Loader_->LoadModel(CardModel4);
	// パーティクル
	Loader_->LoadModel("Particle");
	// スプライトの読み込み
	const std::string& bgSprite = "./Resource/backGround.png";
	Loader_->LoadTexture(bgSprite);
	const std::string& tutorialSprite = "Resource/Tutorial/Tutorial.png";
	const std::string& tutorialArrowSprite = "Resource/Tutorial/Arrow.png";
	Loader_->LoadTexture(tutorialSprite);
	Loader_->LoadTexture(tutorialArrowSprite);
	
	/// ===Sprite=== ///
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize(bgSprite);
	if (sceneManager_->GetLevel() == StageLevel::tutorial) {
		// 生成
		tutorialSprite_ = std::make_unique<Sprite>();
		tutorialArrowSprite_ = std::make_unique<Sprite>();
		tutorialbgSprite_ = std::make_unique<Sprite>();
		// 初期化
		// tutorialSprite
		tutorialSprite_->Initialize(tutorialSprite);
		tutorialSprite_->SetPosition({ 640.0f, 360.0f });
		tutorialSprite_->SetAnchorPoint({ 0.5f, 0.5f });
		tutorialSprite_->SetSize({ 500.0f, 500.0f });
		// tutorialArrowSprite
		tutorialArrowSprite_->Initialize(tutorialArrowSprite);
		tutorialArrowSprite_->SetPosition({ 410.0f, 130.0f });
		tutorialArrowSprite_->SetAnchorPoint({ 0.5f, 0.5f });
		tutorialArrowSprite_->SetSize({ 30.0f, 30.0f });
		// tutorialBGSprite
		tutorialbgSprite_->Initialize(bgSprite);
		tutorialbgSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.9f });

		// モードの設定
		mode_ = Tutorial::Sprite;
	}


	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = { 0.0f, 0.0f, -70.0f };
	cameraRotate_ = { 0.0f, 0.0f, 0.0f };
	cameraScale_ = { 0.0f, 0.0f, 0.0f };
	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);
	cameraManager_->SetActiveCamera("main1");

	// GlobalVariablesの取得
	globalVariables = GlobalVariables::GetInstance();
	// カードデータの登録
	const char* groupNameCards = "Cards";
	globalVariables->CreateGroup(groupNameCards);

	/// ===Model=== ///
	// モデルの生成
	cardManager_ = std::make_unique<CardManager>();
	// ステージの設定
	auto it = cardDatas_.begin();
	// ステージ番号分、要素数を進める
	std::advance(it, static_cast<int>(sceneManager_->GetLevel()));
	// カードマネージャクラスを初期化
	cardManager_->Initialize(*it, cameraManager_.get());
	// デバッグ用ImGui２も設定
	globalVariables->SetValue("Cards", "CardGrid", *it);

	// デバッグ表示用に GlobalVariables にデータを登録
	std::vector<int32_t> cardGrid;
	for (int y = 0; y < cardManager_->GetRows(); ++y) {
		for (int x = 0; x < cardManager_->GetCols(); ++x) {
			cardGrid.push_back(cardManager_->GetCards()[y][x]->GetCardType());
		}
	}
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
	// Tutorialの場合
	if (sceneManager_->GetLevel() == StageLevel::tutorial && mode_ == Tutorial::Sprite) {
		// マウスの処理
		mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
		mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

		// カードマネージャの更新
		cardManager_->Update(mousePosition_);

		// Spriteの更新
		sprite_->Update();
		tutorialSprite_->Update();
		tutorialArrowSprite_->Update();
		tutorialbgSprite_->Update();

		//cameraManager_->SetActiveCamera("main1");
		camera_->SetTranslate(cameraPos_);
		cameraManager_->UpdateAllCameras();

		// デバッグ用ImGui情報
		globalVariables->Update();

		// spriteArrowとの当たり判定を行い当たったらmodeをPlayに変える
		if(ChaekCollisisonTutorial()){
		   mode_ = Tutorial::Play;
		}

	} else {

		// マウスの処理
		mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
		mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

		// カードマネージャの更新
		cardManager_->Update(mousePosition_);

		// Spriteの更新
		sprite_->Update();

		//cameraManager_->SetActiveCamera("main1");
		camera_->SetTranslate(cameraPos_);
		cameraManager_->UpdateAllCameras();

		globalVariables->Update();

		/// ===シーン変更=== ///
		if (cardManager_->AllCardsObtained()) {
			// すべてのカードが obtained ならシーンを変更
			sceneManager_->ChangeScene("Clear");
		} else if (cardManager_->Checkmate()) {
			// 詰みだったらTitleにシーン変更
			sceneManager_->ChangeScene("Title");
		}
	}
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画

	sprite_->Draw(GroundType::Back);

#pragma endregion

#pragma region モデル描画

	cardManager_->Darw();

#pragma endregion

#pragma region 前景スプライト描画
	// tutorialの処理
	if (sceneManager_->GetLevel() == StageLevel::tutorial && mode_ == Tutorial::Sprite) {
		tutorialbgSprite_->Draw();
		tutorialSprite_->Draw();
		tutorialArrowSprite_->Draw();
	}
#pragma endregion
}

void GameScene::RefreshCardData() {
	const char* stageNames[] = {
		"tutorial", "card3x3_1", "card3x3_2", "card3x3_3", "card3x3_4", "card4x4_1", "card4x4_2", "card4x4_3", "card5x5_1", "card5x5_2", "card5x5_3", "card5x5_4",
	};

	if (ImGui::BeginCombo("Select Stage", stageNames[static_cast<int>(sceneManager_->GetLevel())])) {

		if (ImGui::Selectable("tutorial")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::tutorial));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_1")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_2")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_3")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_4")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_4));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_1")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_2")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_3")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_1")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_2")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_3")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_4")) {
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_4));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		ImGui::EndCombo();
	}
}

// 当たり判定
bool GameScene::ChaekCollisisonTutorial() {

	Vector2 diffVector = tutorialArrowSprite_->GetPosition() - mousePosition_;
	float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);
	if (len < 15.0f) {
		if (Mii::TriggerMouse(MouseButtonType::Left)) {
			return true;
		}
	}
	return false;
}
