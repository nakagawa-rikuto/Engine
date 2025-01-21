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
	const std::string& CardModel = "Card";
	Loader_->LoadModel(CardModel);

	/// ===Camera=== ///
	// Camera情報
	cameraPos_ = {5.0f, 9.0f, -70.0f};
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = {0.0f, 0.0f, 0.0f};

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);

	/// ===Model=== ///
	// const int gridSize = 5;                       // グリッドのサイズ
	const float spacing = 5.0f;                   // モデル間の間隔
	const Vector3 basePosition(0.0f, 0.0f, 0.0f); // 基準となる位置

	for (zIndex = 0; zIndex < 5; ++zIndex) {
		for (xIndex = 0; xIndex < 5; ++xIndex) {

			Vector3 position(basePosition.x + xIndex * spacing, basePosition.y + zIndex * spacing, basePosition.z);

			cards_.card[zIndex][xIndex] = std::make_shared<Card>();
			cards_.card[zIndex][xIndex]->Initialize(CardModel, xIndex, position, cameraManager_->GetActiveCamera());
		}
	}

	// GlobalVariablesの取得
	globalVariables = GlobalVariables::GetInstance();

	// カードデータの登録
	const char* groupNameCards = "Cards";
	globalVariables->CreateGroup(groupNameCards);

	// デバッグ表示用に GlobalVariables にデータを登録
	std::vector<int32_t> cardGrid;
	for (int z = 0; z < gridSize; ++z) {
		for (int x = 0; x < gridSize; ++x) {
			cardGrid.push_back(cards_.card[z][x]->GetCardType());
		}
	}
	globalVariables->SetValue("Cards", "CardGrid", cardGrid);
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

	if (Mii::TriggerKey(DIK_A)) {
		--xIndex;
	}
	if (Mii::TriggerKey(DIK_D)) {
		++xIndex;
	}
	if (Mii::TriggerKey(DIK_W)) {
		++zIndex;
	}
	if (Mii::TriggerKey(DIK_S)) {
		--zIndex;
	}
	xIndex = std::clamp(xIndex, 0, 4);
	zIndex = std::clamp(zIndex, 0, 4);

	if (Mii::TriggerKey(DIK_SPACE)) {
		cards_.card[zIndex][xIndex]->RequestState(Card::CardState::front);
	}

	mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

	CheckCursorCardCollision();

	for (int z = 0; z < 5; ++z) {
		for (int x = 0; x < 5; ++x) {
			if (cards_.card[z][x]->GetCurrentState() != Card::CardState::obtained) {
				cards_.card[z][x]->Update(cameraManager_->GetActiveCamera());

				cards_.card[z][x]->SetScale({1.0f, 1.0f, 1.0f});
			}
		}
	}

	CheckFrontPair();

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	// camera_->SetRotate(cameraRotate_);
	cameraManager_->UpdateAllCameras();

	globalVariables->Update(gridSize);
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画

	for (int z = 0; z < 5; ++z) {
		for (int x = 0; x < 5; ++x) {
			cards_.card[z][x]->Draw();
		}
	}

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

void GameScene::CheckFrontPair() {
	// 記録用変数
	int cardType[2];
	int cardX[2];
	int cardZ[2];

	int pairIndex = 0;

	for (int z = 0; z < 5; ++z) {
		for (int x = 0; x < 5; ++x) {
			// 表を向いた状態だったら変数に記録
			if (cards_.card[z][x]->GetCurrentState() == Card::CardState::front) {
				cardType[pairIndex] = cards_.card[z][x]->GetCardType();
				cardX[pairIndex] = x;
				cardZ[pairIndex] = z;

				if (pairIndex == 2) {
					break;
				}

				++pairIndex;
			}
		}
	}

	if (pairIndex == 2) {

		if (cardType[0] == cardType[1]) {

			for (int i = 0; i < 2; ++i) {

				cards_.card[cardZ[i]][cardX[i]]->SetCurrentState(Card::CardState::show);

				cards_.card[cardZ[i]][cardX[i]]->RequestState(Card::CardState::obtained);
			}

			EightDirectionCheck(cardZ, cardX);
		}
		else if (cardType[0] != cardType[1]) {

			for (int i = 0; i < 2; ++i) {

				cards_.card[cardZ[i]][cardX[i]]->SetCurrentState(Card::CardState::show);

				cards_.card[cardZ[i]][cardX[i]]->RequestState(Card::CardState::back);

			}
		}
	}
}

void GameScene::CheckCursorCardCollision() {

	if (!CountStateCard(Card::CardState::front) || !CountStateCard(Card::CardState::show)) {
		return;
	}

	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 5; ++x) {
			Vector2 diffVector = cards_.card[y][x]->GetScreenPosition() - mousePosition_;

			float len = sqrtf(diffVector.x * diffVector.x + diffVector.y * diffVector.y);

			if (len < 25.0f) {
				cards_.card[y][x]->SetScale({1.3f, 1.3f, 1.3f});

				if (Mii::PushMouse(MouseButtonType::Left)) {
					if (TriggerLeft_) {

						cards_.card[y][x]->RequestState(Card::CardState::front);

						TriggerLeft_ = false;
					} else {
						TriggerLeft_ = true;
					}
				}
			}
		}
	}
}

bool GameScene::CountStateCard(Card::CardState state) {
	int count = 0;
	for (int y = 0; y < 5; ++y) {
		for (int x = 0; x < 5; ++x) {
			if (cards_.card[y][x]->GetCurrentState() == state || cards_.card[y][x]->GetRequestState() == state) {
				++count;
			}
		}
	}

	if (count < 2) {
		return true;
	}

	return false;
}

void GameScene::EightDirectionCheck(int yIndex[2], int xIndex[2])
{
	// 差
	int diffY = yIndex[1] - yIndex[0];
	int diffX = xIndex[1] - xIndex[0];

	// 行が同じ場合
	if (diffY == 0) {

		if (std::abs(diffX) > 1) {
			int addNum = diffX / std::abs(diffX);

			int addXIndex = xIndex[0];

			for (int i = 0; i < std::abs(diffX) - 1; ++i) {

				addXIndex += addNum;

				if (cards_.card[yIndex[0]][addXIndex]->GetCurrentState() == Card::CardState::back)
				{
					cards_.card[yIndex[0]][addXIndex]->SetCurrentState(Card::CardState::show);

					cards_.card[yIndex[0]][addXIndex]->RequestState(Card::CardState::obtained);
				}
			}
		}
	}
	// 列が同じ場合
	else if (diffX == 0) {

		if (std::abs(diffY) > 1) {
			int addNum = diffY / std::abs(diffY);

			int addYIndex = yIndex[0];

			for (int i = 0; i < std::abs(diffY) - 1; ++i) {

				addYIndex += addNum;

				if (cards_.card[addYIndex][xIndex[0]]->GetCurrentState() == Card::CardState::back)
				{
					cards_.card[addYIndex][xIndex[0]]->SetCurrentState(Card::CardState::show);

					cards_.card[addYIndex][xIndex[0]]->RequestState(Card::CardState::obtained);
				}
			}
		}
	}
	// 斜めだった場合
	else if (std::abs(diffY) == std::abs(diffX)) {

		int yAdd = diffY / std::abs(diffY);
		int xAdd = diffX / std::abs(diffX);

		for (int i = 0; i < std::abs(diffY) - 1; ++i) {
			yIndex[0] += yAdd;
			xIndex[0] += xAdd;

			if (cards_.card[yIndex[0]][xIndex[0]]->GetCurrentState() == Card::CardState::back)
			{
				cards_.card[yIndex[0]][xIndex[0]]->SetCurrentState(Card::CardState::show);

				cards_.card[yIndex[0]][xIndex[0]]->RequestState(Card::CardState::obtained);
			}
		}
	}
}
