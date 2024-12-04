#include "SceneManager.h"

#include <string>
#include <cassert>

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
void SceneManager::ChangeScene(SceneType sceneType) {
	// 新しいシーンを生成
	currentScene_ = CreateScene(sceneType);
	// 新しいシーンにSceneManagerをセット
	if (currentScene_) {
		currentScene_->SetSceneManager(this);
	}
	Initialize();
}

///-------------------------------------------/// 
/// シーン生成
///-------------------------------------------///
std::unique_ptr<IScene> SceneManager::CreateScene(SceneType sceneType) {
	if (currentScene_) {currentScene_.reset();}
	/// ===シーンの判断=== ///
	switch (sceneType) {
		// タイトルシーン
	case SceneManager::kTitle:
		return std::make_unique<TitleScene>();
		// セレクトシーン
	case SceneManager::kSelect:
		return std::make_unique<SelectScene>();
		// ゲームシーン
	case SceneManager::kGame:
		return std::make_unique<GameScene>();
		// クリアシーン
	case SceneManager::kClear:
		return std::make_unique<ClearScene>();
		// ゲームオーバー
	case SceneManager::kGameOver:
		return std::make_unique<GameOverScene>();
		// 他の新タイプ
	default:
		return nullptr;
	}
}

///-------------------------------------------/// 
/// シーン監視
///-------------------------------------------///
void SceneManager::SceneObservation() {
#ifdef _DEBUG
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
		if (ImGui::Selectable("Title Scene", currentScene_ && dynamic_cast<TitleScene*>(currentScene_.get()))) {
			ChangeScene(SceneType::kTitle);
		}
		///セレクトシーン
		if (ImGui::Selectable("Select Scene", currentScene_ && dynamic_cast<SelectScene*>(currentScene_.get()))) {
			ChangeScene(SceneType::kSelect);
		}
		///ゲームシーン
		if (ImGui::Selectable("Game Scene", currentScene_ && dynamic_cast<GameScene*>(currentScene_.get()))) {
			ChangeScene(SceneType::kGame);
		}
		///クリアシーン
		if (ImGui::Selectable("Clear Scene", currentScene_ && dynamic_cast<ClearScene*>(currentScene_.get()))) {
			ChangeScene(SceneType::kClear);
		}
		///ゲームオーバー
		if (ImGui::Selectable("GameOver Scene", currentScene_ && dynamic_cast<GameOverScene*>(currentScene_.get()))) {
			ChangeScene(SceneType::kGameOver);
		}
		ImGui::EndCombo();
	}
#endif // DEBUGv
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void SceneManager::SetSelectedLevel(int level) { selectLevel_ = level; }

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
int SceneManager::GetSelectedLevel() const { return selectLevel_; }
