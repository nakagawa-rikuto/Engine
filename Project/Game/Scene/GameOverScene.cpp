#include "GameOverScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameOverScene::~GameOverScene() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameOverScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();
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
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameOverScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
