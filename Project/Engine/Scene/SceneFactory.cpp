#include "SceneFactory.h"

// 各シーン
#include "application/Game/Scene/DebugScene.h"
#include "application/Game/Scene/TitleScene.h"
#include "application/Game/Scene/SelectScene.h"
#include "application/Game/Scene/GameScene.h"
#include "application/Game/Scene/ClearScene.h"
#include "application/Game/Scene/GameOverScene.h"

///-------------------------------------------/// 
/// シーン生成
///-------------------------------------------///
std::unique_ptr<IScene> SceneFactory::CreateScene(SceneType type) {
	/// ===シーンの判断=== ///
	if (type == SceneType::Title) {
		// タイトルシーン
		return std::make_unique<TitleScene>();
	} else if (type == SceneType::Select) {
		// セレクトシーン
		return std::make_unique<SelectScene>();
	} else if (type == SceneType::Game) {
		//　ゲームシーン
		return std::make_unique<GameScene>();
	} else if (type == SceneType::Clear) {
		//　クリアシーン
		return std::make_unique<ClearScene>();
	} else if (type == SceneType::GameOver) {
		//　ゲームオーバー
		return std::make_unique<GameOverScene>();
	} else if (type == SceneType::Debug) {
		// デバッグ
		return std::make_unique<DebugScene>();
	}
	return nullptr;
}
