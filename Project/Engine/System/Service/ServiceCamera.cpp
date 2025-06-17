#include "ServiceCamera.h"
// CameraManager
#include "Engine/System/Managers/CameraManager.h"
// ServiceLovator
#include "ServiceLocator.h"

// 追加
void ServiceCamera::Add(const std::string& name, std::shared_ptr<Camera> camera) {
		ServiceLocator::GetCameraManager()->Add(name, camera);
}
// 削除
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
