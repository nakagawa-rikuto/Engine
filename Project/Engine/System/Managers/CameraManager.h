#pragma once
/// ===Include=== ///
// Engine
#include "application/Game/Camera/Camera.h"
#include "application/Game/Camera/FollowCamera.h"
// C++
#include <unordered_map>
#include <string>
#include <memory>

///=====================================================/// 
/// カメラマネージャ
///=====================================================///
class CameraManager {
public:/// ===基本的な関数=== ///

	CameraManager();
	~CameraManager();

	/// <summary>
	/// 全てのカメラの更新
	/// </summary>
	void UpdateAllCameras();

public:/// ===関数=== ///

	/// <summary>
	/// カメラを追加
	/// </summary>
	void AddCamera(const std::string& name, std::shared_ptr<Camera> camera);

	/// <summary>
	/// 追従カメラを追加
	/// </summary>
	void AddFollowCamera(const std::string& name, std::shared_ptr<FollowCamera> followCamera);

	/// <summary>
	/// カメラを削除
	/// </summary>
	void RemoveCamera(const std::string& name);

	/// <summary>
	/// カメラが存在するかチェック
	/// </summary>
	bool HasCamera(const std::string& name) const;

public:/// ===Setter・Getter=== ///

	// アクティブカメラのGetter
	std::shared_ptr<Camera> GetActiveCamera() const;

	// 指定されたカメラのGetter
	std::shared_ptr<Camera> GetCamera(const std::string& name) const;

	// 指定された追従カメラのGetter
	std::shared_ptr<FollowCamera> GetFollowCamera(const std::string& name) const;

	// アクティブカメラのSetter
	void SetActiveCamera(const std::string& name);

private:
	// カメラを管理するマップ
	std::unordered_map<std::string, std::shared_ptr<Camera>> cameras_;

	// 現在アクティブなカメラ
	std::shared_ptr<Camera> activeCamera_;
};