#include "GameOverScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"
#include "Engine/Service/Loader.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameOverScene::~GameOverScene() { sprite_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameOverScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// テクスチャの読み込み
	const std::string& sprite = "./Resource/GameOverScene.png";
	Loader::LoadTexture("GameOver", sprite);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("GameOver");                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameOverScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameOverScene");
	ImGui::End();
#endif // USE_IMGUI

	/// ===Spriteの更新=== ///
	sprite_->Update();

	// シーン切り替え
	if (Mii::TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene(SceneType::Title);
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameOverScene::Draw() {
#pragma region 背景スプライト描画
	// Spriteの描画
	sprite_->Draw();
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
