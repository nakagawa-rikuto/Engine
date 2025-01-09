#include "SceneManager.h"
// c++
#include <cassert>
// 各シーン
#include "Game/Scene/DebugScene.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/SelectScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/ClearScene.h"
#include "Game/Scene/GameOverScene.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
SceneManager::~SceneManager() {
	currentScene_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void SceneManager::Initialize() {
	// シーンの確認
	if (currentScene_) {
		// 現在のシーンの初期化
		currentScene_->Initialize();
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SceneManager::Update() {
	// シーンの確認
	if (currentScene_) {
		// 現在のシーンの更新
		currentScene_->Update();
	}

	// ImGui
	SceneObservation();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SceneManager::Draw() {
	// シーンの確認
	assert(currentScene_);
	if (currentScene_) {
		// 現在のシーンの描画
		currentScene_->Draw();
	}
}

///-------------------------------------------/// 
/// シーン変更
///-------------------------------------------///
void SceneManager::ChangeScene(const std::string& sceneName) {
	// 現在のシーンを更新
	currentSceneName_ = sceneName;
	// 新しいシーンを生成
	if (currentScene_) { currentScene_.reset(); }
	currentScene_ = sceneFactory_->CreateScene(currentSceneName_);
	// 新しいシーンにSceneManagerをセット
	if (currentScene_) {
		currentScene_->SetSceneManager(this);
	}
	Initialize();
}

///-------------------------------------------/// 
/// シーン監視
///-------------------------------------------///
void SceneManager::SceneObservation() {
#ifdef USE_IMGUI
	ImGui::Begin("Scene Manager");
	/// ===現在のシーンを表示=== /// 
	if (currentScene_) {
		std::string currentSceneName = typeid(*currentScene_).name();
		ImGui::Text("Current Scene: %s", currentSceneName.c_str());
	} else {
		ImGui::Text("No Current Scene");
	}

	/// ===シーン変更用のドロップダウンメニュー=== ///
	if (ImGui::BeginCombo("Select Scene", "Change Scene")) {
		///タイトルシーン
		if (ImGui::Selectable("Title Scene", currentSceneName_ == "Title")) {
			ChangeScene("Title");
		}
		///セレクトシーン
		if (ImGui::Selectable("Select Scene", currentSceneName_ == "Select")) {
			ChangeScene("Select");
		}
		///ゲームシーン
		if (ImGui::Selectable("Game Scene", currentSceneName_ == "Game")) {
			ChangeScene("Game");
		}
		///クリアシーン
		if (ImGui::Selectable("Clear Scene", currentSceneName_ == "Clear")) {
			ChangeScene("Clear");
		}
		///ゲームオーバー
		if (ImGui::Selectable("GameOver Scene", currentSceneName_ == "GameOver")) {
			ChangeScene("GameOver");
		}
		///デバッグシーン
		if (ImGui::Selectable("Debug Scene", currentSceneName_ == "Debug")) {
			ChangeScene("Debug");
		}
		ImGui::EndCombo();
	}
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void SceneManager::SetSelectedLevel(int level) { selectLevel_ = level; }
void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory) { sceneFactory_ = sceneFactory; }

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
int SceneManager::GetSelectedLevel() const { return selectLevel_; }
