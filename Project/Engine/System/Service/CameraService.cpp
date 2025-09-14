#include "CameraService.h"
// CameraManager
#include "Engine/System/Managers/CameraManager.h"
// ServiceLovator
#include "ServiceLocator.h"

// 追加(Camera)
void CameraService::AddCamera(const std::string& name, std::shared_ptr<Camera> camera) {
	ServiceLocator::GetCameraManager()->AddCamera(name, camera);
}
// 追加(FollowCamera)
void CameraService::AddFollowCamera(const std::string& name, std::shared_ptr<FollowCamera> FollowCamera) {
	ServiceLocator::GetCameraManager()->AddCamera(name, FollowCamera);
}
// 削除
void CameraService::Remove(const std::string& name) {
	ServiceLocator::GetCameraManager()->RemoveCamera(name);
}
// カメラのチェック
void CameraService::HasCamera(const std::string& name) {
	ServiceLocator::GetCameraManager()->HasCamera(name);
}
// アクティブカメラのGetter
std::shared_ptr<Camera> CameraService::GetActiveCamera() {
	return ServiceLocator::GetCameraManager()->GetActiveCamera();
}
// アクティブカメラのSetter
void CameraService::SetActiveCamera(const std::string& name) {
	ServiceLocator::GetCameraManager()->SetActiveCamera(name);
}
