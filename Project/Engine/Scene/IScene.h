#pragma once
/// ===Include=== ///
#include "Engine/System/Managers/CameraManager.h"
#include "Engine/System/Managers/ParticleManager.h"

/// ===前方宣言=== ///
class SceneManager;

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

public:/// ===Setter=== ///

	void SetSceneManager(SceneManager* sceneManager);

protected:
	// シーンマネージャ
	SceneManager* sceneManager_ = nullptr;
	// Camera
	std::shared_ptr<Camera> defaultCamera_;
};

