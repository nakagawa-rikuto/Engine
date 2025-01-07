#include "TitleScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"

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
	Loader_->LoadTexture(sprite);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
	sprite_->SetTexture(sprite);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void TitleScene::Update() {
	/// ===デバック用ImGui=== ///

	// シーン切り替え
	if (Mii::TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene("Game");
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
