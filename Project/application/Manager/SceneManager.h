#pragma once
/// ===Include=== ///
// c++
#include <memory>
#include <string>
// Game
#include "Engine/Scene/AbstractSceneFactory.h"
// Imgui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

enum class StageLevel {
	tutorial,
	card3x3_1,
	card3x3_2,
	card3x3_3,
	card3x3_4,
	card4x4_1,
	card4x4_2,
	card4x4_3,
	card5x5_1,
	card5x5_2,
	card5x5_3,
	card5x5_4,

	COUNT,
};

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

	// ステージれべルのSetter
	void SetLevel(StageLevel level);
	// ステージレベルのGetter
	StageLevel GetLevel();

public:/// ===シーン処理=== ///

	// シーンの変更
	void ChangeScene(const std::string& sceneName);
	// シーン監視
	void SceneObservation();
	// レベル番号の設定
	void SetSelectedLevel(int level);
	// 選択されたレベル番号の取得
	int GetSelectedLevel() const;
	// シーンファクトリーのSetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);

private:
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// 現在のシーン
	std::string currentSceneName_ = "Title";  // 初期のシーン
	std::unique_ptr<IScene> currentScene_; // 現在のシーン

	// レベル
	StageLevel level_ = StageLevel::tutorial;

	// 選択されたレベル番号を保持する
	int selectLevel_ = 1;
};

