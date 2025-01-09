#include "SelectScene.h"
// SceneManager
#include "Game/Manager/SceneManager.h"

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
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SelectScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
