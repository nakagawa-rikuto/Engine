#pragma once
/// ===Include=== ///
#include "application/Game/Camera/Camera.h"
#include "Engine/DataInfo/LevelData.h"

/// ===前方宣言=== ///
class SceneManager;
class Model;

///=====================================================/// 
/// シーンの基底クラス
///=====================================================///
class IScene {
public:

	IScene() = default;
	virtual ~IScene();

	virtual void Initialize();
	virtual void Update();
	virtual void Draw();

	// 配置関数(Level)
	void LoadModelsForLevel(const std::string& file_name);
	void PlaceLevelObjects();

public:/// ===Setter=== ///

	void SetSceneManager(SceneManager* sceneManager);

protected:
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// Camera
	std::shared_ptr<Camera> defaultCamera_;

private:

	// レベルオブジェクトのインスタンス一覧
	std::vector<Model*> objects_;
	// モデルのキャッシュ（fileNameごとに1つ）
	std::map<std::string, Model*> models_;
	// レベルデータ
	LevelData* levelData_ = nullptr; 
	// レベルが読み込まれたかどうか
	bool IsLevelLoaded_ = false; 
};

