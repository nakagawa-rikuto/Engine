#pragma once
/// ===Include=== ///
#include "Game/Manager/CameraManager.h"
#include "Engine/Core/Mii.h"
#include "Game/Load/AssetLoader.h"
#include "Game/Audio/Audio.h"

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
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// Camera
	std::unique_ptr<Camera> defaultCamera_;
	std::unique_ptr<CameraManager> cameraManager_;
	// Load
	std::unique_ptr<AssetLoader> Loader_;
	// Audio
	std::unique_ptr<Audio> audio_;
};

