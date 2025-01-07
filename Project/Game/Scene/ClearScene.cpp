#include "ClearScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ClearScene::~ClearScene() { sprite_.reset(); }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ClearScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// テクスチャの読み込み
	const std::string& sprite = "./Resource/GameClearScene.png";
	Loader_->LoadTexture(sprite);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize();                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
	sprite_->SetTexture(sprite);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ClearScene::Update() {
	/// ===デバック用ImGui=== ///


	// シーン切り替え
	if (Mii::TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene("Title");
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ClearScene::Draw() {
#pragma region 背景スプライト描画
	// Spriteの描画
	sprite_->Draw();
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
