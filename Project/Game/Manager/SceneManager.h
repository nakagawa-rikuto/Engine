#pragma once
/// ===Include=== ///
// c++
#include <memory>
// Game
#include "Game/Scene/IScene.h"
// Imgui
#ifdef _DEBUG
#include <imgui.h>
#endif // DEBUG

///=====================================================/// 
/// シーンマネージャー
///=====================================================///
class SceneManager {
public:/// ===基本的な関数=== ///

	SceneManager() = default;
	~SceneManager();

	// 初期化
	void Initialize();
	// 現在のシーンの更新
	void Update();
	// 現在のシーンの描画
	void Draw();

public:/// ===シーン処理=== ///

	// シーンの種類
	enum SceneType {
		kDebug,
		kTitle,
		kSelect,
		kGame,
		kClear,
		kGameOver,
	};

	// シーンの変更
	void ChangeScene(SceneType sceneType);
	// シーン監視
	void SceneObservation();
	// レベル番号の設定
	void SetSelectedLevel(int level);
	// 選択されたレベル番号の取得
	int GetSelectedLevel() const;

private:

	// 現在のシーン
	SceneType currentSceneType_ = kTitle;  // 初期のシーン
	std::unique_ptr<IScene> currentScene_; // 現在のシーン

	// 選択されたレベル番号を保持する
	int selectLevel_ = 1;

private:
	// シーンの生成
	std::unique_ptr<IScene> CreateScene(SceneType sceneType);
};

