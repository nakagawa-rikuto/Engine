#include "ClearScene.h"
// SceneManager
#include "application/Manager/SceneManager.h"
#include "Engine/Core/Mii.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ClearScene::~ClearScene() {}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ClearScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ClearScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("ClearScene");

	ImGui::Text("Clear!!!!!");

	ImGui::Separator();

	ImGui::Text("Space To TitleScene");

	ImGui::End();
#endif // USE_IMGUI

	if (Mii::TriggerKey(DIK_SPACE)) {
		sceneManager_->ChangeScene("Title");
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ClearScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}
