#include "GameScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"

#include "Engine/Core/Mii.h"

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
	cameraPos_ = { 0.0f, 0.0f, -70.0f };
	cameraRotate_ = {0.0f, 0.0f, 0.0f};
	cameraScale_ = { 0.0f, 0.0f, 0.0f };

	camera_ = std::make_shared<Camera>();
	camera_->Initialize();
	camera_->SetTranslate(cameraPos_);
	cameraManager_->Add("main1", camera_);


	/// ===Model=== ///
	const int gridSize = 5;  // グリッドのサイズ
	const float spacing = 5.0f;  // モデル間の間隔
	const Vector3 basePosition(0.0f, 0.0f, 0.0f); // 基準となる位置

	for (zIndex = 0; zIndex < 5; ++zIndex)
	{
		for (xIndex = 0; xIndex < 5; ++xIndex)
		{

			Vector3 position(
				basePosition.x + xIndex * spacing,
				basePosition.y + zIndex * spacing,
				basePosition.z 
			);

			cards_.card[zIndex][xIndex] = std::make_shared<Card>();
			cards_.card[zIndex][xIndex]->Initialize(CardModel, xIndex, position, cameraManager_->GetActiveCamera());
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

	if (Mii::TriggerKey(DIK_SPACE))
	{
		cards_.card[zIndex][xIndex]->RequestState(Card::CardState::front);
	}

	for (int z = 0; z < 5; ++z)
	{
		for (int x = 0; x < 5; ++x)
		{
			cards_.card[z][x]->Update(cameraManager_->GetActiveCamera());
		}
	}

	CheckFrontPair();

	cameraManager_->SetActiveCamera("main1");
	camera_->SetTranslate(cameraPos_);
	camera_->SetRotate(cameraRotate_);
	//camera_->SetScale(cameraScale_);
	cameraManager_->UpdateAllCameras();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
	
	for (int z = 0; z < 5; ++z)
	{
		for (int x = 0; x < 5; ++x)
		{
			cards_.card[z][x]->Draw();
		}
	}

#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

void GameScene::CheckFrontPair()
{
	// 記録用変数
	int cardType[2];
	int cardX[2];
	int cardZ[2];

	int pairIndex = 0;


	for (int z = 0; z < 5; ++z)
	{
		for (int x = 0; x < 5; ++x)
		{
			// 表を向いた状態だったら変数に記録
			if (cards_.card[z][x]->GetCurrentState() == Card::CardState::front)
			{
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

	if (pairIndex == 2)
	{
		for (int i = 0; i < 2; ++i)
		{
			if (cardType[0] == cardType[1]) {
				cards_.card[cardZ[i]][cardX[i]]->RequestState(Card::CardState::obtained);
			}
			else if (cardType[0] != cardType[1]) {
				cards_.card[cardZ[i]][cardX[i]]->RequestState(Card::CardState::back);
			}
		}

		
		
	}

}
