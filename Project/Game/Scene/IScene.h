#pragma once
/// ===Include=== ///
#include "Game/Manager/CameraManager.h"
#include "Engine/Core/Mii.h"

/// ===前方宣言=== ///
class SceneManager;

///=====================================================/// 
/// シーンの基底クラス
///=====================================================///
class IScene {
public:

	IScene() = default;
	virtual ~IScene() = default;

	virtual void Initialize();
	virtual void Update() = 0;
	virtual void Draw() = 0;

public:/// ===Setter=== ///

	void SetSceneManager(SceneManager* sceneManager);

protected:
	// input
	Input* input_ = nullptr;
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// Camera
	std::unique_ptr<Camera> defaultCamera_;
	std::unique_ptr<CameraManager> cameraManager_;
};

