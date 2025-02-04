#include "TitleScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"
#include "Engine/Service/Loader.h"
#include "Engine/Service/Input.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
TitleScene::~TitleScene() { sprite_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// テクスチャの読み込み
	const std::string& sprite = "./Resource/TitleScreen.png";
	Loader::LoadTexture("Title", sprite);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("Title");                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void TitleScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("TitleScene");
	ImGui::End();
#endif // USE_IMGUI

	/// ===Spriteの更新=== ///
	sprite_->Update();

	// シーン切り替え
	if (Input::TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene(SceneType::Game);
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void TitleScene::Draw() {
#pragma region 背景スプライト描画
	// Spriteの描画
	sprite_->Draw();
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
