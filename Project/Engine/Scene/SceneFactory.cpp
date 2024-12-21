#include "SceneFactory.h"

// 各シーン
#include "Game/Scene/DebugScene.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/SelectScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/Scene/ClearScene.h"
#include "Game/Scene/GameOverScene.h"

///-------------------------------------------/// 
/// シーン生成
///-------------------------------------------///
std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName) {
	/// ===シーンの判断=== ///
	if (sceneName == "Title") {
		// タイトルシーン
		return std::make_unique<TitleScene>();
	} else if (sceneName == "Select") {
		// セレクトシーン
		return std::make_unique<SelectScene>();
	} else if (sceneName == "Game") {
		//　ゲームシーン
		return std::make_unique<GameScene>();
	} else if (sceneName == "Clear") {
		//　クリアシーン
		return std::make_unique<ClearScene>();
	} else if (sceneName == "GameOver") {
		//　ゲームオーバー
		return std::make_unique<GameOverScene>();
	} else if (sceneName == "Debug") {
		// デバッグ
		return std::make_unique<DebugScene>();
	}
	return nullptr;
}
