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

	/// ===Situationの初期化=== ///
	situation_ = GameSituation::Play;

	/// ===BGM=== ///
	audio_->PlayeSound("GamePlay", true);


	/// ===Sprite=== ///
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("Resource/backGround.png");
	sprite_->Update();
	if (sceneManager_->GetLevel() == StageLevel::tutorial) {
		// 生成
		tutorialSprite_ = std::make_unique<Sprite>();
		tutorialArrowSprite_ = std::make_unique<Sprite>();
		tutorialbgSprite_ = std::make_unique<Sprite>();
		// 初期化
		// tutorialSprite
		tutorialSprite_->Initialize("Resource/Tutorial/Tutorial.png");
		tutorialSprite_->SetPosition({ 640.0f, 360.0f });
		tutorialSprite_->SetAnchorPoint({ 0.5f, 0.5f });
		tutorialSprite_->SetSize({ 500.0f, 500.0f });
		// tutorialArrowSprite
		tutorialArrowSprite_->Initialize("Resource/Tutorial/Arrow.png");
		tutorialArrowSprite_->SetPosition({ 410.0f, 130.0f });
		tutorialArrowSprite_->SetAnchorPoint({ 0.5f, 0.5f });
		tutorialArrowSprite_->SetSize({ 30.0f, 30.0f });
		// tutorialBGSprite
		tutorialbgSprite_->Initialize("Resource/backGround.png");
		tutorialbgSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.9f });

		tutorialSprite_->Update();
		tutorialArrowSprite_->Update();
		tutorialbgSprite_->Update();

		// モードの設定
		mode_ = Tutorial::Sprite;
	} else {
		// ミッション
		const std::string missionSprites_[11] = {
			"./Resource/Mission/mission1.png", "./Resource/Mission/mission1.png",  "./Resource/Mission/mission3.png", "./Resource/Mission/mission4.png",
			"./Resource/Mission/mission5.png", "./Resource/Mission/mission6.png",  "./Resource/Mission/mission7.png", "./Resource/Mission/mission8.png",
			"./Resource/Mission/mission9.png", "./Resource/Mission/mission10.png", "./Resource/Mission/mission11.png"
		};

		for (int i = 0; i < 11; i++) {
			Loader_->LoadTexture(missionSprites_[i]);
		}

		missionSprite_ = std::make_unique<Sprite>();
		missionSprite_->Initialize(missionSprites_[static_cast<int>(sceneManager_->GetLevel()) - 1]);
		missionSprite_->SetPosition({ 20.0f,20.0f });
		missionSprite_->SetSize({ 305.0f, 75.0f });
		missionSprite_->Update();
	}

	/// ===Sitation=== ///
	situationBGSprite_ = std::make_unique<Sprite>();
	clearSprite_ = std::make_unique<Sprite>();
	gameOverSprite_ = std::make_unique<Sprite>();
	titleSprite_ = std::make_unique<Sprite>();
	retrySprite_ = std::make_unique<Sprite>();
	selectSprite_ = std::make_unique<Sprite>();
	// BG
	situationBGSprite_->Initialize("Resource/backGround.png");
	situationBGSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.9f });
	situationBGSprite_->Update();
	// Title
	titleSprite_->Initialize("Resource/Scene/TitleSelect.png");
	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	titleSprite_->SetPosition({ 900.0f, 500.0f });
	titleSprite_->SetSize({ 100.0f, 100.0f });
	titleSprite_->Update();
	// Retry
	retrySprite_->Initialize("Resource/Scene/Retry.png");
	retrySprite_->SetAnchorPoint({ 0.5f, 0.5f });
	retrySprite_->SetPosition({ 640.0f, 500.0f });
	retrySprite_->SetSize({ 100.0f, 100.0f });
	retrySprite_->Update();
	// Select
	selectSprite_->Initialize("Resource/Scene/Select.png");
	selectSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_->SetPosition({ 380.0f, 500.0f });
	selectSprite_->SetSize({ 100.0f, 100.0f });
	selectSprite_->Update();
	// Clear
	clearSprite_->Initialize("Resource/gameClear.png");
	clearSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	clearSprite_->SetPosition({ 640.0f, 200.0f });
	clearSprite_->SetSize({ 700.0f, 150.0f });
	clearSprite_->Update();
	// GameOver
	gameOverSprite_->Initialize("Resource/gameOver.png");
	gameOverSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	gameOverSprite_->SetPosition({ 640.0f, 200.0f });
	gameOverSprite_->SetSize({ 700.0f, 150.0f });
	gameOverSprite_->Update();


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

	const std::string& fadeTexture = "./Resource/back_white.png";
	Loader_->LoadTexture(fadeTexture);

	// フェード
	fade_ = std::make_unique<Fade>();

	fade_->Initialize(fadeTexture);

	fade_->Start(Fade::Status::FadeIn, fadeTimer_);
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
	ImGui::Begin("Mouse");
	ImGui::Checkbox("TriggerRight", &TriggerLeft_);
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

	
	// フェードの更新
	fade_->Update();

	switch (phase_) {
	case Phase::kFadeIn:
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		/*if (Mii::TriggerMouse(MouseButtonType::Left)) {
			TriggerLeft_ = true;
		} else {
			TriggerLeft_ = false;
		}*/

		// マウスの処理
		mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
		mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

		// シーンの状態にる処理
		switch (situation_) {
		case GameScene::GameSituation::Play:
			// Tutorialの場合
			if (sceneManager_->GetLevel() == StageLevel::tutorial && mode_ == Tutorial::Sprite) {

				// カードマネージャの更新
				cardManager_->Update(mousePosition_);

				// Spriteの更新
				sprite_->Update();
				tutorialSprite_->Update();
				tutorialArrowSprite_->Update();
				tutorialbgSprite_->Update();

				// cameraManager_->SetActiveCamera("main1");
				camera_->SetTranslate(cameraPos_);
				cameraManager_->UpdateAllCameras();

				// デバッグ用ImGui情報
				globalVariables->Update();

				// spriteArrowとの当たり判定を行い当たったらmodeをPlayに変える
				if (ChaekCollisisonTutorial()) {
					mode_ = Tutorial::Play;
				}

			}
			else {

				// カードマネージャの更新
				cardManager_->Update(mousePosition_);

				if (cardManager_->GetIsFlip()) {
					audio_->PlayeSound("flipCard", false);
					cardManager_->SetFlag(false);
				}

				// Spriteの更新
				sprite_->Update();

				// cameraManager_->SetActiveCamera("main1");
				camera_->SetTranslate(cameraPos_);
				cameraManager_->UpdateAllCameras();

				globalVariables->Update();

				// すべてのカードが obtained ならシーンを変更
				if (cardManager_->AllCardsObtained()) {
					CheckStarFlag();
				}

				/// ===シーン変更=== ///
				if (Mii::TriggerKey(DIK_ESCAPE)) {
					audio_->PlayeSound("clock", true);

					audio_->StopSound("GamePlay");

					situation_ = GameSituation::Pause;
				}
				if (cardManager_->AllCardsObtained()) {
					audio_->PlayeSound("GCbgm", true);

					audio_->StopSound("GamePlay");

					situation_ = GameSituation::GameClear;
				}
				else if (cardManager_->Checkmate()) {
					audio_->PlayeSound("GObgm", true);

					audio_->StopSound("GamePlay");

					situation_ = GameSituation::GameOver;
				}
			}
			break;
		case GameScene::GameSituation::Pause:

			/// ===シーン変更=== ///
			if (Mii::TriggerKey(DIK_ESCAPE)) {
				situation_ = GameSituation::Play;

				audio_->PlayeSound("GamePlay", true);

				audio_->StopSound("clock");
			}

			// スプライトの更新
			retrySprite_->Update();
			titleSprite_->Update();
			selectSprite_->Update();

			/// ===当たり判定の処理=== ///
			if (CheakCollisionSituationRetry()) {
				audio_->StopSound("clock");

				sceneManager_->ChangeScene("Game");
			}
			else if (CheakCollisionSituationSelect()) {
				audio_->StopSound("clock");

				sceneManager_->ChangeScene("Select");
			}
			else if (CheakCollisionSituationTitle()) {
				audio_->StopSound("clock");

				sceneManager_->ChangeScene("Title");
			}

			break;
		case GameScene::GameSituation::GameClear:

			// スプライトの更新
			retrySprite_->Update();
			titleSprite_->Update();
			selectSprite_->Update();

			/// ===当たり判定の処理=== ///
			if (CheakCollisionSituationRetry()) {
				audio_->StopSound("GCbgm");

				sceneManager_->ChangeScene("Game");
			}
			else if (CheakCollisionSituationSelect()) {
				audio_->StopSound("GCbgm");

				sceneManager_->ChangeScene("Select");
			}
			else if (CheakCollisionSituationTitle()) {
				audio_->StopSound("GCbgm");

				sceneManager_->ChangeScene("Title");
			}

			break;
		case GameScene::GameSituation::GameOver:

			// スプライトの更新
			retrySprite_->Update();
			titleSprite_->Update();
			selectSprite_->Update();

			/// ===当たり判定の処理=== ///
			if (CheakCollisionSituationRetry()) {
				audio_->StopSound("GObgm");

				sceneManager_->ChangeScene("Game");
			}
			else if (CheakCollisionSituationSelect()) {
				audio_->StopSound("GObgm");

				sceneManager_->ChangeScene("Select");
			}
			else if (CheakCollisionSituationTitle()) {
				audio_->StopSound("GObgm");

				sceneManager_->ChangeScene("Title");
			}

			break;
		default:
			break;
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
	} else if(sceneManager_->GetLevel() != StageLevel::tutorial){
		missionSprite_->Draw();
	}

	// シーン別の描画処理
	if (situation_ == GameSituation::Pause) {
		situationBGSprite_->Draw();
		titleSprite_->Draw();
		retrySprite_->Draw();
		selectSprite_->Draw();
	} else if (situation_ == GameSituation::GameClear) {
		situationBGSprite_->Draw();
		titleSprite_->Draw();
		retrySprite_->Draw();
		selectSprite_->Draw();
		clearSprite_->Draw();
	} else if (situation_ == GameSituation::GameOver) {
		situationBGSprite_->Draw();
		titleSprite_->Draw();
		retrySprite_->Draw();
		selectSprite_->Draw();
		gameOverSprite_->Draw();
	}

	fade_->Draw();

#pragma endregion
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void GameScene::RefreshCardData() {
	const char* stageNames[] = {
		"tutorial", "card3x3_1", "card3x3_2", "card3x3_3", "card3x3_4", "card4x4_1", "card4x4_2", "card4x4_3", "card5x5_1", "card5x5_2", "card5x5_3", "card5x5_4",
	};

	if (ImGui::BeginCombo("Select Stage", stageNames[static_cast<int>(sceneManager_->GetLevel())])) {

		if (ImGui::Selectable("tutorial")) {
			sceneManager_->SetLevel(StageLevel::tutorial);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::tutorial));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_1")) {
			sceneManager_->SetLevel(StageLevel::card3x3_1);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_2")) {
			sceneManager_->SetLevel(StageLevel::card3x3_2);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_3")) {
			sceneManager_->SetLevel(StageLevel::card3x3_3);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card3x3_4")) {
			sceneManager_->SetLevel(StageLevel::card3x3_4);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card3x3_4));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_1")) {
			sceneManager_->SetLevel(StageLevel::card4x4_1);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_2")) {
			sceneManager_->SetLevel(StageLevel::card4x4_2);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card4x4_3")) {
			sceneManager_->SetLevel(StageLevel::card4x4_3);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card4x4_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_1")) {
			sceneManager_->SetLevel(StageLevel::card5x5_1);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_1));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_2")) {
			sceneManager_->SetLevel(StageLevel::card5x5_2);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_2));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_3")) {
			sceneManager_->SetLevel(StageLevel::card5x5_3);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_3));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		if (ImGui::Selectable("card5x5_4")) {
			sceneManager_->SetLevel(StageLevel::card5x5_4);
			auto it = cardDatas_.begin();
			std::advance(it, static_cast<int>(StageLevel::card5x5_4));
			cardManager_->CardDataRefresh(*it);
			globalVariables->SetValue("Cards", "CardGrid", *it);
		}
		ImGui::EndCombo();
	}
}

///-------------------------------------------/// 
/// ミッション
///-------------------------------------------///
void GameScene::CheckStarFlag()
{
	Mission mission = stageMissions[static_cast<int>(sceneManager_->GetLevel())];


	if (mission.kStepCount > 0)
	{
		if (mission.kStepCount >= cardManager_->GetStepCount())
		{
			if (mission.kEraseAllCount1 > 0 && mission.kEraseAllCount1 <= cardManager_->GetAllObtainedCardCount())
			{
				star1Flag = true;
			}
		} else
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
	} else
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
			} else
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
			} else
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
			} else
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
			} else
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
			} else
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
			} else
			{
				star2Flag = true;
			}
		}
	}
}

///-------------------------------------------/// 
/// 当たり判定
///-------------------------------------------///
// チュートリアル
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
// シチュエーション
bool GameScene::CheakCollisionSituationRetry() {
	Vector2 diffVector = retrySprite_->GetPosition() - mousePosition_;
	float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);
	if (len < 50.0f) {
		retrySprite_->SetColor({ 0.9f, 0.2f, 0.4f, 1.0f });
		if (Mii::TriggerMouse(MouseButtonType::Left)) {
			return true;
		}
	} else {
		retrySprite_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}
	return false;
}
bool GameScene::CheakCollisionSituationTitle() {
	Vector2 diffVector = titleSprite_->GetPosition() - mousePosition_;
	float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);
	if (len < 50.0f) {
		titleSprite_->SetColor({ 0.9f, 0.2f, 0.4f, 1.0f });
		if (Mii::TriggerMouse(MouseButtonType::Left)) {
			return true;
		}
	} else {
		titleSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	return false;
}
bool GameScene::CheakCollisionSituationSelect() {
	Vector2 diffVector = selectSprite_->GetPosition() - mousePosition_;
	float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);
	if (len < 50.0f) {
		selectSprite_->SetColor({ 0.9f, 0.2f, 0.4f, 1.0f });
		if (Mii::TriggerMouse(MouseButtonType::Left)) {
			return true;
		}
	} else {
		selectSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
	return false;
}