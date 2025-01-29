#include "ClearScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"
#include "Engine/Service/Loader.h"

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
	Loader::LoadTexture("GameClear", sprite);

	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("GameClear");                              // BlendMode変更可　sprite->Initialize(BlendMode::kBlendModeAdd);  
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ClearScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("ClearScene");
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
