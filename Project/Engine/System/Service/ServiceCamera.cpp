#include "ServiceCamera.h"
// Manager
#include "Engine/System/Managers/CameraManager.h"
// ServiceLocator
#include "ServiceLocator.h"

void ServiceCamera::Add(const std::string& name, std::shared_ptr<Camera> camera) {
	ServiceLocator::GetCameraManager()->Add(name, camera);
}

void ServiceCamera::Remove(const std::string& name) {
	ServiceLocator::GetCameraManager()->Remove(name);
}

// アクティブカメラのGetter
std::shared_ptr<Camera> ServiceCamera::GetActiveCamera() {
	return ServiceLocator::GetCameraManager()->GetActiveCamera();
}
// アクティブカメラのSetter
void ServiceCamera::SetActiveCamera(const std::string& name) {
	ServiceLocator::GetCameraManager()->SetActiveCamera(name);
}