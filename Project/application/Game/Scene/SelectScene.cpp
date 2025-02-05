#include "SelectScene.h"
// SceneManager
#include "Engine/Core/Mii.h"
#include "application/Manager/SceneManager.h"

#include "Engine/Core/Mii.h"

///-------------------------------------------///
/// デストラクタ
///-------------------------------------------///
SelectScene::~SelectScene() {}

///-------------------------------------------///
/// 初期化
///-------------------------------------------///
void SelectScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	bgSprite_ = std::make_unique<Sprite>();
	bgSprite_->Initialize("Resource/backGround.png");

	const std::string stageSprites[11] = { "./Resource/Stage/Stage1.png", "./Resource/Stage/Stage2.png",  "./Resource/Stage/Stage3.png", "./Resource/Stage/Stage4.png",
										  "./Resource/Stage/Stage5.png", "./Resource/Stage/Stage6.png",  "./Resource/Stage/Stage7.png", "./Resource/Stage/Stage8.png",
										  "./Resource/Stage/Stage9.png", "./Resource/Stage/Stage10.png", "./Resource/Stage/Stage11.png" };

	for (int i = 0; i < 11; i++) {
		Loader_->LoadTexture(stageSprites[i]);
	}

	// 配置用の変数
	const float centerX = 600.0f;  // 画面の中央X座標
	const float startY = 150.0f;   // 最上段のY座標
	const float spacingX = 150.0f; // X方向の間隔
	const float spacingY = 150.0f; // Y方向の間隔

	// 各行の配置数
	const int rowCounts[3] = { 4, 3, 4 };

	// 各スプライトの行と列
	const int positions[11][2] = {
		{0, 0},
		{1, 0},
		{2, 0},
		{3, 0},
		{0, 1},
		{1, 1},
		{2, 1},
		{0, 2},
		{1, 2},
		{2, 2},
		{3, 2}
	};

	for (int i = 0; i < 11; i++) {
		stageSprite_[i] = std::make_unique<Sprite>();
		stageSprite_[i]->Initialize(stageSprites[i]);

		int row = positions[i][1];
		int col = positions[i][0];

		// その行の左端のX座標を計算（中央基準で調整）
		float rowStartX = centerX - ((rowCounts[row] - 1) * spacingX / 2.0f);

		// スプライトのX, Y座標を設定
		float x = rowStartX + col * spacingX;
		float y = startY + row * spacingY;
		stageSprite_[i]->SetPosition({ x, y });
		stageSprite_[i]->SetSize({ 100.0f, 100.0f });
	}

	/// BGM再生
	audio_->PlayeSound("GamePlay", true);
}

///-------------------------------------------///
/// 更新
///-------------------------------------------///
void SelectScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("SelectScene");
	ImGui::End();
#endif // USE_IMGUI

	mousePosition_.x = static_cast<float>(Mii::GetMousePosition().x);
	mousePosition_.y = static_cast<float>(Mii::GetMousePosition().y);

	bgSprite_->Update();

	for (int i = 0; i < 11; i++) {
		stageSprite_[i]->Update();
	}

	if (Mii::PushKey(DIK_SPACE)) {
		sceneManager_->SetLevel(StageLevel::tutorial);
		sceneManager_->ChangeScene("Game");
	}

	// マウスクリック時に当たり判定をチェック
	CheckMouseCollision();
}

///-------------------------------------------///
/// 描画
///-------------------------------------------///
void SelectScene::Draw() {
#pragma region 背景スプライト描画

	bgSprite_->Draw();

#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画

	for (int i = 0; i < 11; i++) {
		stageSprite_[i]->Draw();
	}

#pragma endregion
}

void SelectScene::CheckMouseCollision() {


	for (int i = 0; i < 11; i++) {
		Vector2 spritePos = stageSprite_[i]->GetPosition();
		Vector2 spriteSize = stageSprite_[i]->GetSize();

		// スプライトの中心座標ではなく、左上座標を取得
		float left = spritePos.x;
		float right = spritePos.x + spriteSize.x;
		float top = spritePos.y;
		float bottom = spritePos.y + spriteSize.y;

		if (mousePosition_.x >= left && mousePosition_.x <= right && mousePosition_.y >= top && mousePosition_.y <= bottom) {
			stageSprite_[i]->SetColor({ 0.9f, 0.2f, 0.4f, 1.0f });
			if (Mii::TriggerMouse(MouseButtonType::Left)) {
				sceneManager_->SetLevel(static_cast<StageLevel>(i + 1));
				sceneManager_->ChangeScene("Game");

				/// BGM再生終了
				audio_->StopSound("GamePlay");

				break;
			}
		} else {
			stageSprite_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}

}