#pragma once
// C++
#include <string>
#include <memory>

/// ===前方宣言=== ///
class Camera;

///-------------------------------------------/// 
/// SerciceCamera
///-------------------------------------------///
class ServiceCamera {
public:

	static void Add(const std::string& name, std::shared_ptr<Camera> camera);

	static void Remove(const std::string& name);

	// アクティブカメラのGetter
	static std::shared_ptr<Camera> GetActiveCamera();
	// アクティブカメラのSetter
	static void SetActiveCamera(const std::string& name);
};

