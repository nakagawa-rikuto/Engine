#pragma once
/// ===Include=== ///
// c++
#include <string>
#include <memory>

/// ===前方宣言=== ///
class Camera;
class FollowCamera;

///=====================================================/// 
/// CameraService
///=====================================================///
class CameraService {
public:

	static void AddCamera(const std::string& name, std::shared_ptr<Camera> camera);
	static void AddFollowCamera(const std::string& name, std::shared_ptr<FollowCamera> followCamera);
	static void Remove(const std::string& name);
	static void HasCamera(const std::string& name);

	static std::shared_ptr<Camera> GetActiveCamera();
	static void SetActiveCamera(const std::string& name);
};

